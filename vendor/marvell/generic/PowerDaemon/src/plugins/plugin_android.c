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
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#include "plugin_tags.h"
#include "plugin_android.h"
#include "ppd_component.h"
#include "list.h"

#define AMS_CONFIG_XML

static struct plugin_event_state android_state[] = {
	{
		.keyword = "boot",
		.state = 0,
	},
	{
		.keyword = "screen",
		.state = 1,
	},
	{
		.keyword = "phone",
		.state = 0,
	},
	{
		.keyword = "fm",
		.state = 0,
	},
	{
		.keyword = "booster",
		.state = 0,
	},
	{
		.keyword = "cpu_limit_min",
		.state = 0,
	},
	{
		.keyword = "cpu_limit_max",
		.state = 0,
	},
	{
		.keyword = "gpu_limit_min",
		.state = 0,
	},
	{
		.keyword = "gpu_limit_max",
		.state = 0,
	},
	{
		.keyword = "ddr_limit_min",
		.state = 0,
	},
	{
		.keyword = "ddr_limit_max",
		.state = 0,
	},
	{
		.keyword = "cpu_hotplug",
		.state = 0,
	},
	{ .keyword = NULL}
};

struct android_global_data {
	struct plugin_event_state *states;
	char android_cur_task[MAX_DATA_LEN];
} _plugin_android;
static struct android_global_data *plugin_android = &_plugin_android;

struct monitor_list {
	char name[MAX_DATA_LEN];
	struct LIST tasks;
};

static char monitor_cfg[MAX_DATA_LEN];
struct LIST task_monitor;

PowerPolicyDaemonPlugin *create_plugin (void);

#ifdef AMS_CONFIG_XML
xmlDocPtr ams_config_doc;
/*  Parse task monitor list file
 */
static int init_task_monitor(char *config_file) {
	xmlNodePtr cur_node, sub_node;
	struct NODE *n, *node;
	struct monitor_list *mlist;
	char taskNode[MAX_DATA_LEN];

	/* configuration file */
	clog(LOG_INFO, "reading configuration file %s\n", config_file);
	ams_config_doc = xmlParseFile(config_file);
	if (ams_config_doc == NULL) {
		clog(LOG_ERR, "failed to parse xml file: %s\n", config_file);
		return -1;
	}

	/* start xml parse */
	cur_node = xmlDocGetRootElement(ams_config_doc);
	if (cur_node == NULL) {
		clog(LOG_ERR, "failed to get root element in %s\n", config_file);
		return -1;
	}

	if (xmlStrcmp(cur_node->name, (const xmlChar *)"PowerPolicyDaemonTaskMonitor")) {
		clog(LOG_ERR, "wrong config file, root node != PowerPolicyDaemonTaskMonitor");
		xmlFreeDoc(ams_config_doc);
		return -1;
	}

	/* now scan the tree */
	cur_node = cur_node->xmlChildrenNode;
	while (cur_node) {
		if(cur_node->type != XML_ELEMENT_NODE) {
			cur_node = cur_node->next;
			continue;
		}

		n = node_new(NULL, sizeof(struct monitor_list));
		if (n == NULL) {
			clog(LOG_ERR, "cannot make enough room for a new monitor_list (%s)\n",
					strerror(errno));
			return -1;
		}
		mlist = (struct monitor_list *)n->content;
		strcpy(mlist->name, (const char *)(cur_node->name));

		sub_node = cur_node->xmlChildrenNode;
		while(sub_node) {

			if(sub_node->type != XML_ELEMENT_NODE) {
				sub_node = sub_node->next;
				continue;
			}

			if (xmlStrcmp(sub_node->name, (const xmlChar *)"keyword") == 0) {
				xmlChar *task_name = xmlNodeListGetString(ams_config_doc, sub_node->xmlChildrenNode, 1);
				strcpy(taskNode, (const char *)task_name);
				node = node_new(taskNode, sizeof(taskNode));
				if (node)
					list_append(&mlist->tasks, node);
				
				clog(LOG_DEBUG, "append a task (%s) into list of %s\n", task_name, mlist->name);
				xmlFree(task_name);
			}
			sub_node = sub_node->next;
		}
		list_append(&task_monitor, n);
		clog(LOG_DEBUG, "append a monitor category (%s) \n", mlist->name);
		cur_node = cur_node->next;
	}
	xmlFreeDoc(ams_config_doc);
	return 0;
}
#else
/*  Parse task monitor list file
 */
static int init_task_monitor(char *config) {
	FILE *config_file;
	char buf[MAX_DATA_LEN];
	struct monitor_list list;

	config_file = fopen(config, "r");
	if (!config_file) {
		clog(LOG_ERR, "%s: %s\n", config, strerror(errno));
		return -1;
	}

	list.name[0] = 0;
	while (!feof(config_file)) {
		struct NODE *node;
		char *clean = NULL;
		if (fgets(buf, sizeof(buf), config_file))
			clean = strtok(buf, " \r\n");

		if (!clean || !*clean || *clean == '#')
			continue;
		else if (*clean == '[') {
			if (clean[1] == '/') {
				node = node_new(&list, sizeof(list));
				if (node)
					list_append(&task_monitor, node);
			} else {
				clean = strtok(clean + 1, " ]");
				if (!clean)
					continue;
				strcpy(list.name, clean);
				clog(LOG_DEBUG, "add a monitor list: %s\n",
				     clean);
				list.tasks.first = NULL;
			}
		} else if (list.name[0]) {
			clog(LOG_DEBUG, "append a task (%s) into list of %s\n",
					clean, list.name);
			node = node_new(clean,
					MAX_DATA_LEN-(size_t)(clean-buf));
			if (node)
				list_append(&list.tasks, node);
		}
	}
	fclose(config_file);
	return 0;
}
#endif

static int mark_keyword_status(const char *kw_name) {
	PowerPolicyDaemonPlugin *plugin = create_plugin();
	PowerPolicyDaemonKeyword *ckw = NULL;

	/* mark keyword update status */
	for(ckw = plugin->keywords; ckw->word != NULL; ckw++) {

		/* if keyword corresponds
		 */
		if (strcmp(ckw->word, kw_name) == 0) {
			ckw->isupdate = TRUE;
			break;
		}
	}
	return 0;
}

/*
 *  parse the 'android_state' keyword
 */
static int android_state_parse(const char *ev, void **obj) {
	char line[MAX_DATA_LEN];
	char *keyword;
	struct plugin_event_state *plugin_ev = android_state;

	strncpy(line, ev, sizeof(line));
	if ((keyword = strtok(line, "=")) != NULL) {
		/* whether the keyword is exist in android_state table */
		while(plugin_ev->keyword != NULL) {
			if (!strncmp(plugin_ev->keyword, keyword, strlen(plugin_ev->keyword))) {
				*obj = strdup(ev);
				return 0;
			}
			plugin_ev++;
		}
	}

	clog(LOG_ERR, "couldn't parse %s\n", ev);
	return -1;
}

/*
 *  evaluate the 'android_state' keyword
 */
static int android_state_evaluate(const void *s) {
	char line[MAX_DATA_LEN];
	char *keyword, *value;
	int rule_value;
	struct plugin_event_state *plugin_ev = android_state;


	/* avoid polluting the 's' that is const */
	strncpy(line, (char *)s, sizeof(line));
	keyword = strtok(line, "=");
	value = strtok(NULL, "");
	rule_value = atoi(value);

	while(plugin_ev->keyword != NULL) {
		if (!strncmp(plugin_ev->keyword, keyword, strlen(plugin_ev->keyword))) {
			//special evaluation for booster
			if(strcmp(keyword, "booster") == 0) {
				clog(LOG_INFO, "booster rule bit %d, current booster state 0x%x", rule_value, plugin_ev->state);
				rule_value = 1 << rule_value;
				return (rule_value & plugin_ev->state) ? MATCH : DONT_MATCH;
			}
			else {
				return (rule_value == plugin_ev->state) ? MATCH : DONT_MATCH;
			}
		}
		plugin_ev++;
	}

	clog(LOG_ERR, "SHOULD never goes here, keyword searching is done in android_state_parse\n");
	return DONT_MATCH;
}

/*
 *  parse the 'monitor_task' keyword
 */
static int ams_task_parse(const char *ev, void **obj) {
	LIST_FOREACH_NODE(node, &task_monitor) {
		struct monitor_list *list = node->content;
		if (!strncmp(ev, list->name, MAX_DATA_LEN)) {
			clog(LOG_DEBUG, "found match keyword %s\n",
			     list->name);
			*obj = &list->tasks;
			return 0;
		}
	}
	clog(LOG_ERR, "couldn't parse %s\n", ev);
	return -1;
}

/*
 *  evaluate the 'monitor_task' keyword
 */
static int ams_task_evaluate(const void *s) {
	clog(LOG_DEBUG, "android_cur_task: %s\n", plugin_android->android_cur_task);
	LIST_FOREACH_NODE(node, (const struct LIST *)s) {
		char *task_kw = node->content;
		if (strstr(plugin_android->android_cur_task, task_kw))
			return MATCH;
	}
	return DONT_MATCH;
}

static void android_dummy_free (void __UNUSED__ *obj)
{
}

static PowerPolicyDaemonKeyword kw[] = {
	{
		.word = "android_state",
		.isupdate = 1,
		.parse = &android_state_parse,
		.evaluate = &android_state_evaluate,
	},
	{
		.word = "monitor_task",
		.parse = &ams_task_parse,
		.evaluate = &ams_task_evaluate,
		.free = &android_dummy_free,
	},
	{ .word = NULL, .parse = NULL, .evaluate = NULL, .free = NULL }
};

static int android_init(void) {
	memset(monitor_cfg, 0, MAX_DATA_LEN);
	memset(&task_monitor, 0, sizeof(task_monitor));
	memset(plugin_android, 0, sizeof(_plugin_android));
	return 0;
}

static int android_exit(void) {
	LIST_FOREACH_NODE(node, &task_monitor) {
		struct monitor_list *list = node->content;
		list_free_sublist(&list->tasks, list->tasks.first);
	}
	list_free_sublist(&task_monitor, task_monitor.first);
	return 0;
}

static int android_update(void *data) {
	struct android_event *ev = (struct android_event *)data;
	struct plugin_event_state *plugin_ev = android_state;

	if(data == NULL)
		return -1;

	if (strstr(ev->keyword, "monitor_task")) {
		memset(plugin_android->android_cur_task, 0, MAX_DATA_LEN);
		strcpy(plugin_android->android_cur_task, ev->ev_data.extra);
		clog(LOG_DEBUG,
			"current ams running task %s\n", plugin_android->android_cur_task);
		mark_keyword_status("monitor_task");
		return 0;
	}

	if (ev->ev_type == EVENT_STATE) {
		while(plugin_ev->keyword != NULL) {
			if (!strncmp(plugin_ev->keyword, ev->keyword, strlen(plugin_ev->keyword))) {
				plugin_ev->state = ev->ev_data.state;
			}
			plugin_ev++;
		}
		mark_keyword_status("android_state");
		return 0;
	}

	clog(LOG_NOTICE, "Can't update android event %s\n", ev->keyword);
	return 0;
}

static int android_conf(const char *key, const char *value) {
	if (value == NULL)
		return -1;

	/* start parsing task monitor by ams */
    if(strcmp(key,"monitor_config") == 0) {
		snprintf(monitor_cfg, MAX_DATA_LEN, "/etc/%s", value);
		clog(LOG_DEBUG, "monitor_cfg is %s.\n", monitor_cfg);
		return init_task_monitor(monitor_cfg);
	}

	return 0;
}

static PowerPolicyDaemonPlugin android = {
	.plugin_name		= "android",
	.plugin_tag			= TAG_ANDROID,
	.keywords			= kw,
	.plugin_init		= &android_init,
	.plugin_exit		= &android_exit,
	.plugin_update		= &android_update,
	.plugin_conf		= &android_conf,
};

PowerPolicyDaemonPlugin *create_plugin (void) {
	return &android;
}
