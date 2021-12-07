/*
 *  Copyright (C) 2002-2008  Mattia Dongili <malattia@linux.it>
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
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include "ppd_component.h"
#include "delegate_shcmd.h"

static struct shcmd_global_data *delegate_shcmd = &_delegate_shcmd;
static int delegate_need_apply=0;

/* shcmd delegate APIs */
static int shcmd_init(void) {
    return 0;
}

static int shcmd_exit(void) {
    struct ticket_obj *tmp_obj;
    struct shcmd_state *tmp_state;

    /* free shcmdTicketObj */
    if (LIST_EMPTY(&delegate_shcmd->shcmdTicketObj))
        return 0;
    clog(LOG_INFO, "freeing shcmdTicketObj.\n");
    LIST_FOREACH_NODE(node, &delegate_shcmd->shcmdTicketObj) {
        tmp_obj = (struct ticket_obj*) node->content;
        if(tmp_obj->private_data) {
            struct shcmd_state *tmp_state = (struct shcmd_state *)tmp_obj->private_data;
            list_free_sublist(&tmp_state->exec_cmd, tmp_state->exec_cmd.first);
            free(tmp_obj->private_data);
        }
    }
    list_free_sublist(&(delegate_shcmd->shcmdTicketObj), delegate_shcmd->shcmdTicketObj.first);
    delegate_shcmd->shcmdTicketObj.first = delegate_shcmd->shcmdTicketObj.last = NULL;

    return 0;
}

static int shcmd_parse (void **obj, const char *key, const char *value) {
    struct NODE *n = NULL;
    struct ticket_obj *tmp_obj;
    struct shcmd_state *tmp_state;
    char cmd_node[MAX_CMD_LEN]={0};

    /* allocate memory for ticket obj */
    if(*obj == NULL) {
        n = node_new(NULL, sizeof(struct ticket_obj));
        if (n == NULL) {
            clog(LOG_ERR, "cannot make enough room for a new ticket OBJ (%s)\n",
                    strerror(errno));
            return -1;
        }
        tmp_obj = (struct ticket_obj *)n->content;
        tmp_obj->private_data = malloc(sizeof(struct shcmd_state));;
        if(tmp_obj->private_data != NULL) {
            memset(tmp_obj->private_data, 0, sizeof(struct shcmd_state));
            list_append(&delegate_shcmd->shcmdTicketObj, n);
            *obj = tmp_obj;
        }
        else {
            clog(LOG_ERR, "cannot make enough room for a new shcmd_state struct (%s)\n",
                    strerror(errno));
            node_free(n);
            return -1;
        }
    }

    /* parse delegate setting */
    tmp_obj = (struct ticket_obj *)(*obj);
    clog(LOG_DEBUG, "parsing ticket obj 0x%x\n", tmp_obj);
    tmp_state = (struct shcmd_state *)(tmp_obj->private_data);

    if(strcmp(key, "exec") == 0) {
        strncpy(cmd_node, value, MAX_CMD_LEN);
        n = node_new(cmd_node, sizeof(cmd_node));
        if (n) {
            list_append(&tmp_state->exec_cmd, n);
            clog(LOG_DEBUG, "append SHCMD command [%s].\n", cmd_node);
            return 0;
        }
        else {
            return -1;
        }
    }

    clog(LOG_DEBUG, "SHCMD not supported attributes.\n");
    return -1;

}

static int shcmd_vote (int valid, unsigned int level, void *obj){
    struct ticket_obj *tmp_obj = (struct ticket_obj *)obj;
    if(tmp_obj->valid != valid) {
        tmp_obj->valid = valid;
        tmp_obj->level = level;
        delegate_need_apply++;
    }
    clog(LOG_DEBUG, "SHCMD voting obj=0x%x valid=%d level=%d.\n", tmp_obj, tmp_obj->valid, tmp_obj->level);
    return 0;
}

static int shcmd_apply (int force) {
    struct ticket_obj *tmp_obj;
    struct shcmd_state *tmp_state;
    pid_t child_pid = 0;
    struct sigaction signal_action;
    int child_ret = 0;

    if (LIST_EMPTY(&delegate_shcmd->shcmdTicketObj)) {
        clog(LOG_DEBUG, "SHCMD ticket LIST is empty\n");
        return 0;
    }

    if ((!delegate_need_apply)&&(!force)) {
        clog(LOG_DEBUG, "SHCMD tickets no change, nothing to apply\n");
        return 0;
    }

    {
		/* go through all tickets:
		* exec valid ticket shell command,
		*/
        LIST_FOREACH_NODE(node, &delegate_shcmd->shcmdTicketObj) {
            tmp_obj = (struct ticket_obj*) node->content;
            if(tmp_obj->valid) {
                tmp_state = (struct shcmd_state *)tmp_obj->private_data;
                LIST_FOREACH_NODE(sub_node, &tmp_state->exec_cmd) {
                    char *exec_cmd = sub_node->content;
                    if(exec_cmd) {
                        clog(LOG_DEBUG, "SHCMD EXE: [%s]\n", exec_cmd);
                        switch (child_pid = fork()) {
                            case -1:
                                clog(LOG_ERR, "Unable to fork new process: %s\n",
                                        strerror(errno));
                                break;
                            case 0:
                                clog(LOG_DEBUG, "child process, exec 'sh -c %s'\n",
                                        exec_cmd);
                                /* child */
                                /* reset signal handlers to default */
                                sigemptyset(&signal_action.sa_mask);
                                sigaddset(&signal_action.sa_mask, SIGTERM);
                                sigaddset(&signal_action.sa_mask, SIGINT);
                                sigaddset(&signal_action.sa_mask, SIGHUP);
                                sigaddset(&signal_action.sa_mask, SIGALRM);
                                signal_action.sa_flags = 0;
                                signal_action.sa_handler = SIG_DFL;
                                sigaction(SIGTERM, &signal_action, 0);
                                sigaction(SIGINT, &signal_action, 0);
                                sigaction(SIGHUP, &signal_action, 0);
                                sigaction(SIGALRM, &signal_action, 0);

                                /* TODO: test if file exists, is executable, etc.*/
                                /* perhaps we don't need that, beacause exit status will be logged*/
                                if (!access("/system/bin/sh", X_OK))
                                    child_ret = execl("/system/bin/sh", "/system/bin/sh", "-c", exec_cmd, NULL);
                                clog(LOG_ERR, "Unable to execl new process: %s\n",
                                        strerror(errno));
                                exit(1);
                            default:
                                waitpid(child_pid, &child_ret, 0);
                                if(WIFEXITED(child_ret)) {
                                    clog(LOG_NOTICE, "\"%s\" exited with status %d\n",
                                            exec_cmd, WEXITSTATUS(child_ret));
                                    clog(LOG_DEBUG, "EXE: %s done\n", exec_cmd);
                                } else if(WIFSIGNALED(child_ret)) {
                                    clog(LOG_NOTICE, "\"%s\" exited on signal %d\n",
                                            exec_cmd, WTERMSIG(child_ret));
                                } else {
                                    clog(LOG_WARNING, "\"%s\" exited with status %d\n",
                                            exec_cmd, child_ret);
                                }

                        }
                    }
                }
            }
        }
        /* set shcmd frequency */
        delegate_need_apply = 0;
    }
    return 0;
}


static int shcmd_free (void *obj) {

    return 0;
}


static PowerPolicyDaemonDelegate shcmd = {
    .delegate_name      = "shcmd",
    .delegate_init      = &shcmd_init,
    .delegate_exit      = &shcmd_exit,
    .delegate_parse     = &shcmd_parse,
    .delegate_vote      = &shcmd_vote,
    .delegate_apply     = &shcmd_apply,
    .delegate_free      = &shcmd_free,
};

PowerPolicyDaemonDelegate *create_delegate (void) {
    return &shcmd;
}
