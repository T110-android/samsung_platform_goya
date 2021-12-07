/*
 *  Copyright (C) 2002-2009  Mattia Dongili <malattia@linux.it>
 *                           George Staikos <staikos@0wned.org>
 *  Copyright (C) 2012 Marvell, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "config_parser.h"
#include "ppd.h"
#include "ppd_log.h"
#include "ppd_component.h"
#include "ppd_remote.h"
#include "ppd_plugin_utils.h"
#include "ppd_delegate_utils.h"
#include "daemon_utils.h"
#include "list.h"
#include "sock_utils.h"
#include <sys/resource.h>

#define TRIGGER_RULE_EVENT(event_func, directives, dir, old, new) \
do { \
	LIST_FOREACH_NODE(__node, (directives)) { \
		dir = (struct directive *)__node->content; \
		if (dir->keyword->event_func != NULL) { \
			clog(LOG_DEBUG, "Triggering " #event_func " for %s\n", dir->keyword->word); \
			dir->keyword->event_func(dir->obj, (old), (new)); \
		} \
	} \
} while (0);

#define TRIGGER_PROFILE_EVENT(event_func, directives, dir, old, new, cpu_num) \
do { \
	LIST_FOREACH_NODE(__node, (directives)) { \
		dir = (struct directive *)__node->content; \
		if (dir->keyword->event_func != NULL) { \
			clog(LOG_DEBUG, "Triggering " #event_func " for %s\n", dir->keyword->word); \
			dir->keyword->event_func(dir->obj, (old), (new), (cpu_num)); \
		} \
	} \
} while (0);

#define GET_DELEGATE_BY_NAME(target_delegate, config, name) \
do { \
	PowerPolicyDaemonDelegate *tmp_delegate = NULL; \
	struct delegate_obj *o_delegate = NULL; \
	LIST_FOREACH_NODE(node, &config->delegates) { \
		o_delegate = (struct delegate_obj*)node->content; \
		tmp_delegate = o_delegate->delegate; \
		if (strcmp(tmp_delegate->delegate_name,name) == 0) { \
			target_delegate = tmp_delegate; \
			clog(LOG_DEBUG,"Delegate hit!!! %s\n", target_delegate->delegate_name); \
			break; \
		} else { \
			clog(LOG_DEBUG,"Delegate Miss: %s, name %s\n", tmp_delegate->delegate_name,name); \
		}\
	} \
} while (0);

#define CHECK_DELEGATE_TYPE(type) \
	(strcmp(type,"cpu")==0) || (strcmp(type,"ddr")==0) || \
	(strcmp(type,"vpu")==0) || (strcmp(type,"gpu")==0) || \
	(strcmp(type,"shcmd")==0)

#define REPLY_TO_REMOTE(fd,msg) write(fd,msg,strlen(msg))

#define MAX_ATTR_LEN 128

static int ppd_sock = -1;
static char dirname[MAX_STRING_LEN];
static int force_update = 0;	/* force updating all keyword status and voting */
static int timer_expired = 1;	/* expired in order to run on the first loop */
int ppd_alive = 1;      /* mark ppd alive status */
static pthread_t socket_server_thread;
static pthread_t watchdog_thread;
#define PPD_WATCHDOG_TIMEOUT 60

/* default configuration */
static PowerPolicyDaemonConf default_configuration = {
	.config_file		= PPD_CONFDIR"powerdaemon.xml",
	.pidfile		= PPD_STATEDIR"ppd.pid",
    .operation_mode = MODE_DYNAMIC,
	.poll_intv		= { .tv_sec = DEFAULT_POLL, .tv_usec = 0 },
	.has_sysfs		= 1,
	.no_daemon		= 0,
	.log_level		= DEFAULT_VERBOSITY,
	.enable_remote		= 0,
	.remote_gid		= 0,
	.print_help		= 0,
	.print_version		= 0,
	.android_log		= 0,
};
PowerPolicyDaemonConf *configuration;

/*
 * socket server thread
 */
#define MAX_DATA_ISIZE 256
#define MAX_LISTEN_BACKLOG 16
static int socket_listener_fd=-1;

static int ppd_socket_listener (void) {
	int fd;
	socklen_t alen;
	struct sockaddr_un sck;

	sck.sun_family = AF_UNIX;
	/* Use an abstract socket address */
	sck.sun_path[0] = '\0';
	alen = (socklen_t) strlen(PPD_SOCKET_SVR);
	if (alen > 107)
		alen = 107;
	strncpy(sck.sun_path + 1, PPD_SOCKET_SVR, (size_t) alen);
	sck.sun_path[107] = '\0';
	alen += (socklen_t)(1 + offsetof(struct sockaddr_un, sun_path));

	if (((fd = socket(PF_UNIX, SOCK_STREAM, 0)) == -1)) {
		clog(LOG_ERR, "Couldn't open socket server (%s).\n",
				strerror(errno));
		return -1;
	}

	if (bind(fd, (struct sockaddr *) &sck, alen) == -1) {
		clog(LOG_ERR, "Couldn't bind the PPD socket \"%s\" (%s).\n",
				PPD_SOCKET_SVR, strerror(errno));
		close(fd);
		return -1;
	}

	if (listen(fd, MAX_LISTEN_BACKLOG) == -1) {
		clog(LOG_ERR, "Couldn't listen on the PPD socket"
				"\"%s\" (%s).\n", PPD_SOCKET_SVR, strerror(errno));
		close(fd);
		return -1;
	}

	clog(LOG_NOTICE, "Listen on the PPD socket (%s) successfully\n",
		PPD_SOCKET_SVR);
	return fd;
}

static void *ppd_socket_handler (void *arg) {
	fd_set readfds;
    int tid, ret;
    int read_fd = *(int *)arg;
    int plugin_tag = 0;
    PowerPolicyDaemonConf *config;
    PowerPolicyDaemonPlugin *plugin = NULL;
	struct plugin_obj *o_plugin = NULL;

    tid = pthread_self();
    config = get_ppd_config();
    if(!config) {
        clog(LOG_ERR, "Couldn't open PPD config file.\n");
		goto exit;
    }

	FD_ZERO(&readfds);
	FD_SET(read_fd, &readfds);
	while (1) {
		ssize_t cnt;
		ret = select(read_fd + 1, &readfds, NULL, NULL, NULL);
		if(FD_ISSET(read_fd, &readfds)) {
			char buf[MAX_DATA_ISIZE];
            int head[2];
			int data_len = 0;

			cnt = read(read_fd, head, sizeof(head));
            if(cnt <= 0) {
                clog(LOG_INFO, "no data received from socket fd %d\n", read_fd);
                break;
            }
            if (cnt == sizeof(head)) {
                plugin_tag = head[0];
                data_len = head[1];
                clog(LOG_INFO, "read socket %d, [tag]=%d, [len]=%d.", read_fd, plugin_tag, data_len);

                cnt=0;
                if(data_len > MAX_DATA_ISIZE) {
                    clog(LOG_ERR, "socket data length %d overflow the MAX_DATA_ISIZE, discard it.", data_len);
                    //empty the socket buffer
                    while(cnt < data_len){
                        cnt += read(read_fd, buf, MAX_DATA_ISIZE);
                    }
                    continue;
                }
                else {
                    while(cnt < data_len){
                        cnt += read(read_fd, buf+cnt, (size_t)(data_len-cnt));
                    }
                }

//                pthread_cleanup_push(pthread_mutex_unlock, (void *)&config->ppd_mutex);
                pthread_mutex_lock(&config->ppd_mutex);
                /* plugin update */
                LIST_FOREACH_NODE(node, &config->plugins) {
                    o_plugin = (struct plugin_obj *)node->content;
                    if(o_plugin->plugin->plugin_tag == (unsigned int)plugin_tag) {
                        plugin = o_plugin->plugin;
                        break;
                    }
                }

                if((plugin != NULL)&&(plugin->plugin_update != NULL)) {
                    plugin->plugin_update(buf);
                }
                pthread_mutex_unlock(&config->ppd_mutex);
//                pthread_cleanup_pop(0);

              /* Ring the bell!! */
              wakeup_ppd();
           } else {
             clog(LOG_INFO, "Got dirty data header size %d, [tag]=%d, drop it.", cnt, plugin_tag);
             break;
           }
    }
  }

    /* notify the socket thread exit */
    if((plugin != NULL)&&(plugin->plugin_sock_release != NULL)) {
        plugin->plugin_sock_release(tid);
    }

exit:
	close(read_fd);
	clog(LOG_INFO, "socket fd %d closed\n", read_fd);
	return NULL;
}

static void *socket_server_func (void __UNUSED__ *arg) {
	clog(LOG_INFO, "PPD socket server thread running.\n");

    /* Clear SIGALRM/SIGTERM, as they should be handler in main thread */
    struct sigaction signal_action;
    sigemptyset(&signal_action.sa_mask);
    sigaddset(&signal_action.sa_mask, SIGALRM | SIGTERM);
    sigprocmask(SIG_BLOCK, &signal_action.sa_mask, NULL);

	while (1) {
		socket_listener_fd = ppd_socket_listener();

		while (socket_listener_fd > 0) {
			struct sockaddr_un peer;
			int ret, thread_svr_fd;
			socklen_t len = sizeof(peer);
			pthread_t socket_handler_thread;

			thread_svr_fd = accept(socket_listener_fd, (struct sockaddr *)&peer, &len);
			if (thread_svr_fd < 0) {
				continue;
			}
			clog(LOG_DEBUG, "Create a socket handler thread. fd = %d\n", thread_svr_fd);

			if ((ret = pthread_create(&socket_handler_thread, NULL,
                    &ppd_socket_handler, &thread_svr_fd))) {
				clog(LOG_ERR, "Unable to launch socket client thread: %s\n",
                    strerror(ret));
				continue;
			}
		}
		close(socket_listener_fd);
	}
	return NULL;
}

static void* watchdog_func (__attribute__((unused)) void *arg) {
    /* Clear SIGALRM/SIGTERM, as they should be handler in main thread */
    struct sigaction signal_action;
    sigemptyset(&signal_action.sa_mask);
    sigaddset(&signal_action.sa_mask, SIGALRM | SIGTERM);
    sigprocmask(SIG_BLOCK, &signal_action.sa_mask, NULL);

    while (1) {
        if(configuration && configuration->operation_mode == MODE_DYNAMIC) {
            if(ppd_alive) {
                ppd_alive = 0;
            }
            else {
			    clog(LOG_NOTICE, "%d watchdog timeout, killing powerdaemon...", gettid());
                kill_ppd();
            }
        }
        usleep(PPD_WATCHDOG_TIMEOUT*1000*1000);
    }
	return NULL;
}

/* update keywords status in each rule */
static int rule_update(PowerPolicyDaemonRule *rule) {
       struct directive *d = NULL;
	int need_update=0;

	LIST_FOREACH_NODE(node, &rule->directives) {
		d = (struct directive *) node->content;
		/* compute scores for rules and keep the highest */
		if ((d->keyword->isupdate)||(d->keyword->polling)) {
			need_update = TRUE;
			break;
		}
	}
	return need_update;
}

/*
 * Evaluates the full rule and returns the percentage score
 * for it.
 */
static int rule_evaluate(PowerPolicyDaemonRule *rule) {
	unsigned int hits = 0, directives = 0;
	struct directive *d = NULL;

	/* call plugin->evaluate for each rule */
	LIST_FOREACH_NODE(node, &rule->directives) {
		d = (struct directive *) node->content;
		/* compute scores for rules and keep the highest */
		if (d->keyword->evaluate != NULL) {
			directives++;
			if (d->keyword->evaluate(d->obj) == MATCH) {
				hits++;
				clog(LOG_DEBUG, "Rule \"%s\": <%s> match.\n", rule->rule_name,d->keyword->word);
			}
			else
				clog(LOG_DEBUG, "Rule \"%s\": <%s> dismatch.\n", rule->rule_name,d->keyword->word);
		}
	} /* end foreach rule entry */

    if ((hits > 0) && (directives == hits))
        return TRUE;
    else
        return FALSE;
}

static int set_ppd_runmode(int mode) {
	if (mode == MODE_DYNAMIC) {
		if((configuration->poll_intv.tv_usec > 0)||(configuration->poll_intv.tv_sec > 0)) {
			struct itimerval new_timer;
			new_timer.it_interval.tv_usec = configuration->poll_intv.tv_usec;
			new_timer.it_interval.tv_sec = configuration->poll_intv.tv_sec;
			new_timer.it_value.tv_usec = configuration->poll_intv.tv_usec;
			new_timer.it_value.tv_sec = configuration->poll_intv.tv_sec;
			/* set next alarm */
			if (setitimer(ITIMER_REAL, &new_timer, 0) < 0) {
				clog(LOG_CRIT, "Couldn't set timer: %s\n", strerror(errno));
				return errno;
			}
		}
//		timer_expired = 1;
	}
	else if (mode == MODE_MANUAL) {
		/* reset alarm */
		if (setitimer(ITIMER_REAL, NULL, 0) < 0) {
			clog(LOG_CRIT, "Couldn't set timer: %s\n", strerror(errno));
			return errno;
		}
	}
	else {
		clog(LOG_WARNING, "Unknown mode %d\n", mode);
		return EINVAL;
	}
	configuration->operation_mode = mode;
	return 0;
}

/*  int read_args (int argc, char *argv[])
 *  Reads command line arguments
 */
static int read_args (int argc, char *argv[]) {

	static struct option long_options[] = {
		{ "help",	0, 0, 'h' },
		{ "version",	0, 0, 'v' },
		{ "file",	1, 0, 'f' },
		{ "no-daemon",	0, 0, 'D' },
		{ "manual",	0, 0, 'm' },
		{ "android-log",0, 0, 'A' },
		{ 0, 0, 0, 0 },
	};
	int ch,option_index = 0;

	while ((ch = getopt_long(argc, argv, "hvf:DmV:A", long_options, &option_index)) != -1) {
		switch (ch) {
		case '?':
		case 'h':
			configuration->print_help = 1;
			return 0;
		case 'v':
			configuration->print_version = 1;
			return 0;
		case 'f':
			if (realpath(optarg, configuration->config_file) == NULL) {
				clog(LOG_ERR, "Error reading command line argument (%s: %s).\n",
						optarg, strerror(errno));
				return -1;
			}
			clog(LOG_DEBUG, "Using configuration file at %s\n", configuration->config_file);
			break;
		case 'D':
			configuration->no_daemon = 1;
			break;
		case 'm':
			configuration->operation_mode = MODE_MANUAL;
			return 0;
		case 'A':
			configuration->android_log = 1;
			break;
		default:
			break;
		}
	}
	return 0;
}

/*
 * Prints program version
 */
static void print_version(const char *me) {
	printf("%s version "__PPD_VERSION__".\n", me);
	printf("Copyright 2002-2009 Mattia Dongili <"__PPD_MAINTAINER__">\n"
	       "                    George Staikos <staikos@0wned.org>\n");
}

/*  void print_help(const char *me)
 *  Prints program help
 */
static void print_help(const char *me) {
	printf("Usage: %s [OPTION]...\n\n"
			"  -h, --help                   display this help and exit\n"
			"  -v, --version                display version information and exit\n"
			"  -f, --file                   config file (default: "PPD_CONFIG")\n"
			"  -D, --no-daemon              stay in foreground and print log to stdout (used to debug)\n"
			"  -m, --manual                 start in manual mode (ignored if the enable_remote is 0)\n"
			"  -V, --verbosity              verbosity level from 0 (less verbose) to 7 (most verbose)\n"
			"  -A, --android-log            dump output to android in-kernel log buffer\n"
			"\n"
			"Report bugs to Mattia Dongili <"__PPD_MAINTAINER__">.\n", me);
}

static void term_handler(int signo) {
	clog(LOG_NOTICE, "%d Caught %s signal (%s), forcing exit.\n", gettid(), __func__, strsignal(signo));
    if (configuration != NULL) {
        /* For safe ops, make powerdaemon as manual mode */
        set_ppd_runmode(MODE_MANUAL);
        if (ppd_sock != -1) {
			close_unix_sock(ppd_sock);
			delete_temp_dir(dirname);
        }

        /* FIXME: To avoid race condition, let Linux recycle the resources. */
        //free_configuration(configuration);
        //free(configuration);
    }
	clog(LOG_NOTICE, "%d--Hey, I`m going to quit!!!", gettid());
    exit(0);
}

static void alarm_handler(int signo) {
	clog(LOG_INFO, "%d Caught ALARM signal (%s).\n", gettid(), strsignal(signo));
	timer_expired++;
}

static void hup_handler(int signo) {
#if 0
	clog(LOG_NOTICE, "Caught HUP signal (%s), reloading configuration file.\n", strsignal(signo));
	force_reinit = 1;
#else
	clog(LOG_WARNING, "Caught HUP signal (%s), ignored.\n", strsignal(signo));
#endif
}

static void pipe_handler(int signo) {
  clog(LOG_NOTICE, "Caught PIPE signal (%s).\n", strsignal(signo));
}

static void ppd_loop(PowerPolicyDaemonConf *config) {
    PowerPolicyDaemonRule *tmp_rule;
    PowerPolicyDaemonConstraint *tmp_constraint;
    PowerPolicyDaemonDelegate *tmp_delegate;
	PowerPolicyDaemonKeyword *ckw = NULL;
	struct ticket *tmp_ticket = NULL;
	struct delegate_obj *o_delegate = NULL;
	struct plugin_obj *o_plugin = NULL;
    int rule_hit, rule_need_update;

    /* update timestamp */
    if (gettimeofday(&configuration->timestamp, NULL) < 0) {
    	clog(LOG_ERR, "Couldn't read current time: %s\n", strerror(errno));
    } else {
    	clog(LOG_DEBUG, "Current time is: %lu::%lu\n",
    			configuration->timestamp.tv_sec,
    			configuration->timestamp.tv_usec);
    }

    /* pre change event */

    /* constraint voting */
	clog(LOG_INFO, "<========= start constraint voting =========>");
	pthread_mutex_lock(&config->ppd_mutex);
    LIST_FOREACH_NODE(node, &config->rules) {
        tmp_rule = (PowerPolicyDaemonRule *)node->content;
		rule_need_update = rule_update(tmp_rule);
        if(rule_need_update || force_update) {
            rule_hit = rule_evaluate(tmp_rule);
            tmp_constraint = tmp_rule->constraint;
            tmp_constraint->active = rule_hit;
		    clog(LOG_DEBUG, "Considering Rule <%s>, hit = %d\n", tmp_rule->rule_name, rule_hit);

			if(rule_hit) {
				clog(LOG_NOTICE, "Hit Rule <%s> ==> Constraint <%s>\n", tmp_rule->rule_name, tmp_rule->constraint_name);
			}

            LIST_FOREACH_NODE(n, &tmp_constraint->tickets) {
                tmp_ticket = (struct ticket *)n->content;
                tmp_delegate = tmp_ticket->delegate;
                if(tmp_delegate->delegate_vote != NULL) {
                    tmp_delegate->delegate_vote(rule_hit, tmp_rule->level, tmp_ticket->obj);
                }
            }
        }
    }

    /* clear all keywords update flag */
    LIST_FOREACH_NODE(node, &config->plugins) {
		o_plugin = (struct plugin_obj*)node->content;
		/* foreach keyword */
		for(ckw = o_plugin->plugin->keywords; ckw->word != NULL; ckw++) {
			ckw->isupdate = FALSE;
		}
	}
	pthread_mutex_unlock(&config->ppd_mutex);

	clog(LOG_INFO, "<========= start delegate apply =========>");
    /* delegate tickets apply */
    LIST_FOREACH_NODE(node, &config->delegates) {
		o_delegate = (struct delegate_obj*)node->content;
		tmp_delegate = o_delegate->delegate;

		if(tmp_delegate->delegate_apply != NULL)
			tmp_delegate->delegate_apply(force_update);
	}
	force_update = 0;

	/* post change event */

	clog(LOG_INFO, "<========= exit ppd loop =========>");
	return;
}

/* NOTICE: When set up to manual mode, We keep the below policy, others would be cancel*/

const char* rule_to_keep [] = {
	"CallIn",
	"CallIdle",
	"AudioPlay",
	"VideoOff",
	"BootComplete",
	NULL,
};

static int is_in_rule_keep_list(char* rule, const char** rule_list) {
	int rule_index = 0, found = -1;
	while (rule_list[rule_index] != NULL) {
		if (strcmp(rule, rule_list[rule_index]) == 0) {//found
			found = 0;
			break;
		}
		rule_index++;
	}
	return found;
}

static void clear_rules() {
	PowerPolicyDaemonRule *tmp_rule,*target_rule=NULL;
	PowerPolicyDaemonConstraint *tmp_constraint;
	PowerPolicyDaemonConf *config = configuration;
	struct ticket *tmp_ticket = NULL;
	LIST_FOREACH_NODE(rule_node, &config->rules) {
		tmp_rule = (PowerPolicyDaemonRule *) rule_node->content;
		if (tmp_rule->constraint->active == 1) { /* current active rule */
			if (is_in_rule_keep_list(tmp_rule->rule_name, rule_to_keep) == 0) {
				continue;//ignore what we want to keep
			}
			target_rule = tmp_rule;
			pthread_mutex_lock(&config->ppd_mutex);
			tmp_constraint = tmp_rule->constraint;
			tmp_constraint->active = 0; /* Clear */
			PowerPolicyDaemonDelegate *tmp_delegate = NULL;
			/* delegate tickets vote */
			LIST_FOREACH_NODE(ticket_node,&tmp_constraint->tickets) {
				tmp_ticket = (struct ticket *) ticket_node->content;
				tmp_delegate = tmp_ticket->delegate;
				if (tmp_delegate->delegate_vote != NULL) {
					tmp_delegate->delegate_vote(FALSE, tmp_rule->level, tmp_ticket->obj);
				}
			}
			pthread_mutex_unlock(&config->ppd_mutex);
			/* delegate tickets apply */
			LIST_FOREACH_NODE(ticket_node, &tmp_constraint->tickets) {
				tmp_ticket = (struct ticket *) ticket_node->content;
				tmp_delegate = tmp_ticket->delegate;
				if (tmp_delegate->delegate_apply!= NULL) {
					tmp_delegate->delegate_apply(0);
				}
			}
		}
	}
}

/*
 * Parse and execute the client command
 */
static void execute_command(int sock, PowerPolicyDaemonConf *config) {
	Command cmd;
	memset(&cmd,0,sizeof(Command));

	struct pollfd fds;
    /* we have a valid sock, wait for command
     * don't wait more tha 0.5 sec
     */
    fds.fd = sock;
    fds.events = POLLIN | POLLRDNORM;

    if (poll(&fds, 1, 500) != 1) {
        clog(LOG_ALERT, "Waited too long for data, aborting.\n");

    } else if (read(sock, &cmd, sizeof(Command)) == -1) {
        clog(LOG_ALERT, "process_packet - read(): %s\n", strerror(errno));

    } else if (cmd.type != INVALID_CMD) {
        clog(LOG_INFO, "command received: %0.4x\n",cmd.type);

        PowerPolicyDaemonDelegate *target_delegate = NULL;
        switch (cmd.type) {
            case CMD_SET_LOG_LEVEL: /*set log level*/
                clog(LOG_DEBUG, "CMD_SET_LOG_LEVEL, argc: %d, args: %s.\n",cmd.argc,cmd.args);

                if ( cmd.args[0] > LOG_DEBUG + '0' || cmd.args[0] < LOG_EMERG + '0' ) {
                    clog(LOG_ALERT, "Error log level %s.\n", cmd.args);
                    REPLY_TO_REMOTE(sock,"Log level set failed, error args!!!\n");
                    return;
                }

                int level = atoi(cmd.args);
                configuration->log_level = level;
                REPLY_TO_REMOTE(sock,"Log level set done!!!\n");
                break;

            case CMD_UPDATE_STATE: /*update state*/
                clog(LOG_DEBUG, "CMD_UPDATE_STATE\n");
                clog(LOG_ALERT, "Ignoring unimplemented command %0.8x\n", cmd.type);
                break;

            case CMD_LIST_RULES: /*list rules*/
                clog(LOG_DEBUG, "CMD_LIST_RULES, argc: %d, args: %s.\n",cmd.argc,cmd.args);
                if (LIST_EMPTY(&config->rules)) {
                    clog(LOG_ALERT, "Empty rule list!!!\n");
                    REPLY_TO_REMOTE(sock,"No Rules!\n");
                } else {
                    PowerPolicyDaemonRule *tmp_rule;
                    PowerPolicyDaemonConstraint *tmp_constraint;
                    char info[MAX_ATTR_LEN];
                    LIST_FOREACH_NODE(node, &config->rules) {
                        memset(info,0,MAX_ATTR_LEN);
                        tmp_rule = (PowerPolicyDaemonRule *) node->content;
                        tmp_constraint = tmp_rule->constraint;
                        clog(LOG_DEBUG, "Rule: %s, enable: %d, constraint: %s, active %d;",
                            tmp_rule->rule_name,tmp_rule->enable,tmp_rule->constraint_name,tmp_constraint->active);
                        snprintf(info,MAX_ATTR_LEN,"Rule: %s, enable: %d, constraint: %s, active %d;\n",
                            tmp_rule->rule_name,tmp_rule->enable,tmp_rule->constraint_name,tmp_constraint->active);
                        REPLY_TO_REMOTE(sock,info);
                    }
                }
                break;

            case CMD_LIST_CONSTRAINTS: /*list constraints*/
                clog(LOG_DEBUG, "CMD_LIST_CONSTRAINTS\n"); /* using CMD_LIST_RULES instead */
                clog(LOG_ALERT, "Ignoring unimplemented command %0.8x\n", cmd.type);
                break;

            case CMD_CUR_RULE: /*cur rule*/
                clog(LOG_DEBUG, "CMD_CUR_RULE, argc: %d, args: %s\n",cmd.argc,cmd.args);
                if (LIST_EMPTY(&config->rules)) {
                    clog(LOG_ALERT, "Empty rule list!!!\n");
                    REPLY_TO_REMOTE(sock,"No Rules!\n");
                } else {
                    PowerPolicyDaemonRule *tmp_rule;
                    PowerPolicyDaemonConstraint *tmp_constraint;
                    char info[MAX_ATTR_LEN];
                    LIST_FOREACH_NODE(node, &config->rules) {
                        memset(info,0,MAX_ATTR_LEN);
                        tmp_rule = (PowerPolicyDaemonRule *) node->content;
                        tmp_constraint = tmp_rule->constraint;
                        if (tmp_constraint->active) { /* hit! */
                            tmp_constraint = tmp_rule->constraint;
                            clog(LOG_DEBUG, "Rule: %s, enable: %d, constraint: %s, active %d;",
                                tmp_rule->rule_name,tmp_rule->enable,tmp_rule->constraint_name,tmp_constraint->active);
                            snprintf(info,MAX_ATTR_LEN,"Rule: %s, enable: %d, constraint: %s, active %d;\n",
                                tmp_rule->rule_name,tmp_rule->enable,tmp_rule->constraint_name,tmp_constraint->active);
                            REPLY_TO_REMOTE(sock,info);
                        }
                    }
                }
                break;

            case CMD_SET_RULE: /*set rule*/
                clog(LOG_DEBUG, "CMD_SET_RULE, argc: %d, args: %s.\n",cmd.argc,cmd.args);
                if (configuration->operation_mode == MODE_DYNAMIC) {
                    clog(LOG_ERR, "Couldn't set rule while running "
                            "in DYNAMIC mode.\n");
                    REPLY_TO_REMOTE(sock,"Couldn't set rule while running in DYNAMIC mode.\n");
                    break;
                }
                if (LIST_EMPTY(&config->rules)) {
                    clog(LOG_ERR, "Empty rule list!!!\n");
                } else {
                    PowerPolicyDaemonRule *tmp_rule,*target_rule=NULL;
                    PowerPolicyDaemonConstraint *tmp_constraint;
                    struct ticket *tmp_ticket = NULL;
                    LIST_FOREACH_NODE(rule_node, &config->rules) {
                        tmp_rule = (PowerPolicyDaemonRule *) rule_node->content;
                        clog(LOG_DEBUG,"Rule: %s,Target: %s.",tmp_rule->rule_name,cmd.args);

                        if (strcmp(tmp_rule->rule_name,cmd.args) == 0) { /* hit! */
                            target_rule = tmp_rule;
                            pthread_mutex_lock(&config->ppd_mutex);
                            tmp_constraint = tmp_rule->constraint;
                            tmp_constraint->active = 1; /* set */
                            PowerPolicyDaemonDelegate *tmp_delegate = NULL;

                            /* delegate tickets vote */
                            LIST_FOREACH_NODE(ticket_node,&tmp_constraint->tickets) {
                                tmp_ticket = (struct ticket *) ticket_node->content;
                                tmp_delegate = tmp_ticket->delegate;
                                if (tmp_delegate->delegate_vote != NULL) {
                                    tmp_delegate->delegate_vote(TRUE, tmp_rule->level, tmp_ticket->obj);
                                }
                            }
                            pthread_mutex_unlock(&config->ppd_mutex);

                            /* delegate tickets apply */
                            LIST_FOREACH_NODE(ticket_node, &tmp_constraint->tickets) {
                                tmp_ticket = (struct ticket *) ticket_node->content;
                                tmp_delegate = tmp_ticket->delegate;
                                if (tmp_delegate->delegate_apply!= NULL) {
                                    tmp_delegate->delegate_apply(0);
                                }
                            }
                        }
                    }
					if (target_rule == NULL) {
						clog(LOG_ALERT, "CMD_SET_RULE, argc: %d, args: %s.Rule not found!!!\n",cmd.argc,cmd.args);
						REPLY_TO_REMOTE(sock,"Rule not found!!!\n");
					}
                }
                break;

			case CMD_CLEAR_RULE: /*clear rule*/
                clog(LOG_DEBUG, "CMD_CLEAR_RULE, argc: %d, args: %s.\n",cmd.argc,cmd.args);
                if (configuration->operation_mode == MODE_DYNAMIC) {
                    clog(LOG_ERR, "Couldn't clear rule while running "
                            "in DYNAMIC mode.\n");
					REPLY_TO_REMOTE(sock,"Couldn't clear rule while running in DYNAMIC mode.\n");
                    break;
                }
                if (LIST_EMPTY(&config->rules)) {
                    clog(LOG_ERR, "Empty rule list!!!\n");
                } else {
                    PowerPolicyDaemonRule *tmp_rule,*target_rule=NULL;
                    PowerPolicyDaemonConstraint *tmp_constraint;
                    struct ticket *tmp_ticket = NULL;
                    LIST_FOREACH_NODE(rule_node, &config->rules) {
						tmp_rule = (PowerPolicyDaemonRule *) rule_node->content;
						clog(LOG_DEBUG,"Rule: %s,Target: %s.",tmp_rule->rule_name,cmd.args);

                        if (strcmp(tmp_rule->rule_name,cmd.args)==0) { /* hit! */
                            target_rule = tmp_rule;
                            pthread_mutex_lock(&config->ppd_mutex);
                            tmp_constraint = tmp_rule->constraint;
                            tmp_constraint->active = 0; /* Clear */
                            PowerPolicyDaemonDelegate *tmp_delegate = NULL;
                            /* delegate tickets vote */
                            LIST_FOREACH_NODE(ticket_node,&tmp_constraint->tickets) {
                                tmp_ticket = (struct ticket *) ticket_node->content;
                                tmp_delegate = tmp_ticket->delegate;
                                if (tmp_delegate->delegate_vote != NULL) {
                                    tmp_delegate->delegate_vote(FALSE, tmp_rule->level, tmp_ticket->obj);
                                }
                            }
                            pthread_mutex_unlock(&config->ppd_mutex);

                            /* delegate tickets apply */
                            LIST_FOREACH_NODE(ticket_node, &tmp_constraint->tickets) {
                                tmp_ticket = (struct ticket *) ticket_node->content;
                                tmp_delegate = tmp_ticket->delegate;
                                if (tmp_delegate->delegate_apply!= NULL) {
                                    tmp_delegate->delegate_apply(0);
                                }
                            }
                        }
                    }
					if (target_rule == NULL ) {
						clog(LOG_ALERT, "CMD_CLEAR_RULE, argc: %d, args: %s. Rule not found!!!\n",cmd.argc,cmd.args);
						REPLY_TO_REMOTE(sock,"Rule not found!!!\n");
					}
                }
                break;

            case CMD_SET_PPD_MODE: /*set ppd mode*/
                clog(LOG_DEBUG, "CMD_SET_PPD_MODE, argc: %d, args: %s.\n",cmd.argc,cmd.args);
				int mode = -1;
				if (strcmp(cmd.args,"auto") == 0) {
					mode = MODE_DYNAMIC;
				} else if ( strcmp(cmd.args,"manual") == 0 ) {
					mode = MODE_MANUAL;
				} else {
					clog(LOG_ALERT, "Illegal type of PPD mode %s.\n", cmd.args);
					REPLY_TO_REMOTE(sock,"Illegal type of PPD mode!!!\n");
					break;
				}
                set_ppd_runmode(mode);
				if (mode == MODE_DYNAMIC) {
					force_update = 1;
					//just set to 1, ignore all alarm counts during manual mode
					timer_expired = 1;
				} else {
					// clear all unnecessary rules when set to be manual.
					clear_rules();
				}
				REPLY_TO_REMOTE(sock,"PPD Mode set done!!!\n");
                break;

            case CMD_SET_CONSTRAINT: /*set constraints*/
                clog(LOG_DEBUG, "CMD_SET_CONSTRAINT\n");//using CMD_SET_RULES instead
                clog(LOG_ALERT, "Ignoring unimplemented command %0.8x\n", cmd.type);
                break;

            case CMD_SET_FREQ: /*set frequency*/
                clog(LOG_DEBUG, "CMD_SET_FREQS, argc: %d, args: %s.\n",cmd.argc,cmd.args);
				int n = 0,delegateId = -1,argc = 0;
				unsigned long freq = 0;
				char *arg = NULL;
				argc = cmd.argc;
				while (n < argc) {
					if (n == 0) { /* <delegates type> */
						arg = strtok(cmd.args," ");
						if (CHECK_DELEGATE_TYPE(arg)) {
							GET_DELEGATE_BY_NAME(target_delegate,config,arg);
						} else
							clog(LOG_ALERT, "Unsupported delegate type. %s.\n",arg);
					} else if (n==1) { /* <delegates Id> */
						arg = strtok(NULL," ");
						if (arg == NULL) {
							 clog(LOG_ALERT, "No delegates Id, CMD_SET_FREQ: <delegates type><delegates Id><freq>\n");
							 return;
						} else {
							delegateId = atoi(arg);
						}
					} else if (n==2) { /* <freq> */
						arg = strtok(NULL," ");
						if (arg == NULL) {
							 clog(LOG_ALERT, "No freq, CMD_SET_FREQ: <delegates type><delegates Id><freq>\n");
							 return;
						} else {
							freq = (unsigned long)atol(arg);
						}
					} else {
						clog(LOG_ALERT, "Ignoring args for CMD_SET_FREQ: <delegates type><delegates Id><freq>\n");
					}
					n++;
				}

                if (target_delegate != NULL) {
					PowerPolicyDaemonDelegateUtils* delegateUtil = (PowerPolicyDaemonDelegateUtils*)(target_delegate->data);
					if (delegateUtil == NULL) {
						clog(LOG_ALERT, "Can not get the utils func for %s.",target_delegate->delegate_name);
						return;
					}
                    int num = (int)delegateUtil->get_unit_num();
					if (delegateId <0 || delegateId >= num) {
						clog(LOG_ALERT, "Can not set freq for delegateId %d\n",delegateId);
						return;
					} else {
						delegateUtil->set_fix_freq((unsigned int)delegateId,freq);
					}
                } else {
                    clog(LOG_ALERT, "Can not find delegate\n");
                }
                break;

            case CMD_GET_SCALING_AVAIL_FREQS: /*get all enable freqs*/
                clog(LOG_DEBUG, "CMD_GET_ALL_ENABLE_FREQS, argc: %d, args: %s.\n",cmd.argc,cmd.args);
				n = 0,delegateId = -1;
				int dump_all_delegate = FALSE;
				arg = NULL;
				argc = cmd.argc;
				while (n < argc) {
					if (n == 0) { /* <delegates type> */
						arg = strtok(cmd.args," ");
						if (CHECK_DELEGATE_TYPE(arg)) {
							GET_DELEGATE_BY_NAME(target_delegate,config,arg);
						} else
							clog(LOG_ALERT, "Unsupported delegate type. %s\n",arg);
					} else if (n==1) { /* <delegates Id> */
						arg = strtok(NULL," ");
						if (arg == NULL)
							 dump_all_delegate = TRUE;
						else
							delegateId = atoi(arg);
					} else {
						clog(LOG_ALERT, "Ignoring args for CMD_GET_SCALING_AVAIL_FREQS: <delegates type><delegates Id>\n");
					}
					n++;
				}

                if (target_delegate != NULL) {
					PowerPolicyDaemonDelegateUtils* delegateUtil = (PowerPolicyDaemonDelegateUtils*)(target_delegate->data);
					if (delegateUtil == NULL) {
						clog(LOG_ALERT, "Can not get the utils func for %s.",target_delegate->delegate_name);
						return;
					}
                    int num = (int)delegateUtil->get_unit_num();
					if (delegateId <0 || delegateId >= num) {
						clog(LOG_ALERT, "Can not set freq for delegateId %d\n",delegateId);
					} else {
						char data[MAX_ATTR_LEN],freqsInfo[MAX_ATTR_LEN];
						for (int i = 0; i<num; i++) {
							if (i == delegateId || dump_all_delegate) {
								memset(data,0,MAX_ATTR_LEN);
								memset(freqsInfo,0,MAX_ATTR_LEN);
								delegateUtil->get_avail_freqs((unsigned int)i,freqsInfo);
	                            snprintf(data, MAX_ATTR_LEN,"%s%d available freqs: %s",target_delegate->delegate_name, i ,freqsInfo);
								clog(LOG_DEBUG, "%s%d available freqs: %s",target_delegate->delegate_name, i, freqsInfo);
								REPLY_TO_REMOTE(sock,data);
							}
						}
					}
                } else {
                    clog(LOG_ALERT, "Can not find delegate\n");
                }
                break;

			case CMD_CUR_PROFILE: /*get cur profile*/
				clog(LOG_DEBUG, "CMD_CUR_PROFILE, argc: %d, args: %s.\n",cmd.argc,cmd.args);
				n = 0,delegateId = -1,dump_all_delegate = FALSE;
				arg = NULL;
				argc = cmd.argc;
				while (n < argc) {
					if (n == 0) { /* <delegates type> */
						arg = strtok(cmd.args," ");
						if (CHECK_DELEGATE_TYPE(arg)) {
							GET_DELEGATE_BY_NAME(target_delegate,config,arg);
						} else
							clog(LOG_ALERT, "Unsupported delegate type. %s.\n",arg);
					} else if (n == 1) { /* <delegates Id> */
						arg = strtok(NULL," ");
						if (arg == NULL)
							 dump_all_delegate = TRUE;
						else
							delegateId = atoi(arg);
					} else {
						clog(LOG_ALERT, "Ignoring args for CMD_CUR_PROFILE: <delegates type><delegates Id>\n");
					}
					n++;
				}

                if (target_delegate != NULL) {
					PowerPolicyDaemonDelegateUtils* delegateUtil = (PowerPolicyDaemonDelegateUtils*)(target_delegate->data);
					if (delegateUtil == NULL) {
						clog(LOG_ALERT, "Can not get the utils func for %s.",target_delegate->delegate_name);
						return;
					}
                    int num = (int)delegateUtil->get_unit_num();
					if (delegateId <0 || delegateId >= num) {
						clog(LOG_ALERT, "Can not get cur profile for delegateId %d\n",delegateId);
					} else {
						char info[MAX_ATTR_LEN];
						unsigned long minfreq,maxfreq,curfreq;
						char governor[16];

						for (int i = 0; i<num; i++) {
							if (i == delegateId || dump_all_delegate) {
								memset(info,0,MAX_ATTR_LEN);
								memset(governor,0,16);
								delegateUtil->get_current_policy((unsigned int)i,&minfreq,&maxfreq,&curfreq,governor);
								snprintf(info,MAX_ATTR_LEN,"%s%d cur profile--min: %ld, max: %ld, cur: %ld, governor: %s",
									target_delegate->delegate_name,i,minfreq,maxfreq,curfreq,governor);
								clog(LOG_DEBUG, "%s%d cur profile--min: %ld, max: %ld, cur: %ld, governor: %s",
									target_delegate->delegate_name,i,minfreq,maxfreq,curfreq,governor);
								REPLY_TO_REMOTE(sock, info);
							}
						}
					}
                } else {
                    clog(LOG_ALERT, "Can not find delegate\n");
                }
				break;

            default:
                clog(LOG_ALERT, "Unable to process packet: %d\n",cmd.type);
                break;
        }
    }
}


PowerPolicyDaemonConf * get_ppd_config(void) {
	return configuration;
}

#if 0
static int get_chip_revision(void) {
    FILE *fp;
    char line[256];
	const char *split = ": ";
	int rev_num = 0;

    fp = fopen("/proc/cpuinfo", "r");
    if(!fp) {
        clog(LOG_ERR, "Cannot open cpuinfo file: %s\n", strerror(errno));
        return -1;
    }

    while(!feof(fp)) {
        fgets(line, 255, fp);
        if(!strncmp(line, "Revision", 8)) {
            char *rev = strtok(line,split);
			if(rev != NULL) {
				rev = strtok(NULL, split);
			}

			if(rev != NULL) {
				rev_num = atoi(rev);
			}
			break;
		}
    }
    fclose(fp);

    clog(LOG_DEBUG, "chip revision number is %i \n", rev_num);

    return rev_num;
}
#endif

/*
 *  main !
 *  Let's go
 */
int main (int argc, char *argv[]) {

	struct sigaction signal_action;
	sigset_t old_sigmask;
	fd_set rfds;
	int peer_sock = -1;
	int ret = 0;
//	int chip_rev;

	configuration = malloc(sizeof(PowerPolicyDaemonConf));
	if (configuration == NULL) {
		ret = ENOMEM;
		goto out;
	}
	memcpy(configuration, &default_configuration, sizeof(PowerPolicyDaemonConf));

	/* Increase the priority of PPD */
	ret = setpriority(PRIO_PROCESS, 0, -20);
	if (ret < 0) {
		ppd_log(LOG_WARNING, "Unable set pri %s.\n", strerror(errno));
	} else {
		ppd_log(LOG_INFO, "Set pri done!");
	}

//	configuration->operation_mode = MODE_DYNAMIC;

	/*
	 *  check perms
	 */
#if 0
	if (geteuid() != 0) {
		ppd_log(LOG_CRIT, "%s: must be run as root.\n", argv[0]);
		ret = EACCES;
		goto out;
	}
#endif

	/*
	 *  read_args
	 */
	if (read_args(argc, argv)!=0) {
		ppd_log(LOG_CRIT, "Unable parse command line parameters, exiting.\n");
		ret = EINVAL;
		goto out;
	}
	if (configuration->print_help) {
		print_help(argv[0]);
		goto out;
	}
	if (configuration->print_version) {
		print_version(argv[0]);
		goto out;
	}

	/* setup signal handlers */
	sigemptyset(&signal_action.sa_mask);
	sigaddset(&signal_action.sa_mask, SIGTERM);
	sigaddset(&signal_action.sa_mask, SIGINT);
	sigaddset(&signal_action.sa_mask, SIGHUP);
	sigaddset(&signal_action.sa_mask, SIGALRM);
	sigaddset(&signal_action.sa_mask, SIGPIPE);
	signal_action.sa_flags = 0;

	signal_action.sa_handler = term_handler;
	sigaction(SIGTERM, &signal_action, 0);

	signal_action.sa_handler = term_handler;
	sigaction(SIGINT, &signal_action, 0);

	signal_action.sa_handler = hup_handler;
	sigaction(SIGHUP, &signal_action, 0);

	signal_action.sa_handler = alarm_handler;
	sigaction(SIGALRM, &signal_action, 0);

	signal_action.sa_handler = pipe_handler;
	sigaction(SIGPIPE, &signal_action, 0);

	/*
	 *  daemonize if necessary
	 */
	if (configuration->no_daemon==0 && daemonize()!=0) {
		clog(LOG_CRIT, "Unable to go background, exiting.\n");
		ret = ECHILD;
		goto out;
	}

ppd_start:

#if 0
	chip_rev = get_chip_revision();
	if(chip_rev >= PXA988_Z3) {
		clog(LOG_INFO, "It's above Z3 revision\n");
		strcpy(configuration->config_file , PPD_CONFDIR"powerdaemon_z3.xml");
	}
#endif

	if (init_configuration(configuration) < 0) {
		clog(LOG_CRIT, "Unable to parse config file: %s\n", configuration->config_file);
		ret = EINVAL;
		goto out_config_read;
	}

	/* setup UNIX socket if necessary */
	if (configuration->enable_remote) {
		dirname[0] = '\0';
		if (create_temp_dir(dirname, configuration->remote_gid) == NULL) {
			clog(LOG_ERR, "Couldn't create temporary directory %s\n", dirname);
			ppd_sock = -1;
		} else if ((ppd_sock = open_unix_sock(dirname, configuration->remote_gid)) == -1) {
			delete_temp_dir(dirname);
			clog(LOG_ERR, "Couldn't open socket, remote controls disabled\n");
		} else {
			clog(LOG_INFO, "Remote controls enabled\n");
			if (configuration->operation_mode == MODE_MANUAL)
				clog(LOG_INFO, "Starting in manual mode\n");
		}
	}

	/* Validate plugins, if none left exit.... */
	if (validate_plugins(&configuration->plugins) == 0) {
		ppd_log(LOG_CRIT, "Hey! all the plugins I loaded are useless, "
				"maybe your configuration needs some rework.\n"
				"Exiting.\n");
		ret = EINVAL;
		goto out_socket;
	}

	/* create socket server thread */
	pthread_mutex_init(&configuration->ppd_mutex,NULL);
	if((ret = pthread_create(&socket_server_thread, NULL, (void *)&socket_server_func, NULL))) {
		clog(LOG_CRIT, "Unable to create socket server thread\n");
		ret = EINVAL;
		goto out_socket;
	}

	if((ret = pthread_create(&watchdog_thread, NULL, watchdog_func, NULL))) {
		clog(LOG_CRIT, "fail to create watchdog thread\n");
		ret = EINVAL;
		goto out_socket;
	}

	/* write pidfile */
	if (write_ppd_pid(configuration->pidfile) < 0) {
		clog(LOG_CRIT, "Unable to write pid file: %s\n", configuration->pidfile);
		ret = EINVAL;
		goto out_socket;
	}

	/* if we are going to pselect the socket
	 * then block all signals to avoid races,
	 * will be unblocked by pselect
	 *
	 * NOTE: Since Linux today does not have a pselect() system  call,  the
	 *       current glibc2 routine still contains this race.
	 *       (man 2 pselect).
	 *       I'll make all the efforts to avoid that race (the code between
	 *       setitimer and pselect is as short as possible, but...)
	 *
	 */
	if (ppd_sock > 0) {
		sigemptyset(&old_sigmask);
		sigemptyset(&signal_action.sa_mask);
		sigaddset(&signal_action.sa_mask, SIGALRM);
		sigprocmask(SIG_BLOCK, &signal_action.sa_mask, &old_sigmask);
	}
	/* otherwise if for any reason the control socket is closed
	 * force operation_mode to dynamic and move on
	 */
	else
		configuration->operation_mode = MODE_DYNAMIC;

	set_ppd_runmode(configuration->operation_mode);
	while (1) {
        ppd_alive = 1;
		if (configuration->operation_mode == MODE_DYNAMIC && timer_expired) {
			ppd_loop(configuration);
			timer_expired--;
		}

		/* if the socket opened successfully */
		if (ppd_sock > 0) {
			/* wait for a command */
			FD_ZERO(&rfds);
			FD_SET(ppd_sock, &rfds);

			if (!timer_expired || configuration->operation_mode == MODE_MANUAL) {
				switch (pselect(ppd_sock+1, &rfds, NULL, NULL, NULL, &old_sigmask)) {
					case 0:
						/* timed out. check to see if things have changed */
						/* will never happen actually... */
						break;
					case -1:
						/* caused by SIGALARM (mostly) log if not so */
						if (errno != EINTR)
							clog(LOG_NOTICE, "pselect(): %s.\n", strerror(errno));
						break;
					case 1:
						/* somebody tried to contact us. see what he wants */
						peer_sock = accept(ppd_sock, NULL, 0);
						if (peer_sock == -1) {
							clog(LOG_ALERT, "Unable to accept connection: "
									" %s\n", strerror(errno));
						}
						execute_command(peer_sock, configuration);
						close(peer_sock);
						peer_sock = -1;
						break;
					default:
						clog(LOG_ALERT, "pselect(): Internal error caught.\n");
						break;
				}
			}
		}
		/* paranoid check for timer expiration
		 * (might actually happen...)
		 */
		else if (!timer_expired) {
			pause();
		}
	}

	/*
	 * Clean up pidfile
	 */
	clear_ppd_pid(configuration->pidfile);

	/* close socket */
out_socket:
	if (ppd_sock != -1) {
		close_unix_sock(ppd_sock);
		delete_temp_dir(dirname);
	}

	/*
	 *  Free configuration structures
	 */
out_config_read:
	free_configuration(configuration);
		ppd_log(LOG_INFO, "Restarting ppd\n");
		goto ppd_start;

out:
	if (configuration != NULL)
		free(configuration);
	/*
	 *  bye bye
	 */
	return ret;
}
