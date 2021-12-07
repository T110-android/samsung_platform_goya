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
#include "plugin_coda.h"
#include "ppd_component.h"
#include "list.h"

struct coda_global_data {
	unsigned int total_instances;
	unsigned int update_instances;
	struct LIST coda_instance_list;
} _plugin_coda;
static struct coda_global_data *plugin_coda = &_plugin_coda;

PowerPolicyDaemonPlugin *create_plugin (void);

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
 *  parse the 'coda_info' keyword
 */
static int coda_info_parse(const char *ev, void **obj) {
	char line[MAX_DATA_LEN];
	char *keyword, *value;

	strncpy(line, ev, sizeof(line));
	keyword = strtok(line, "=");
	value = strtok(NULL, "");
	if((keyword == NULL)||(value == NULL))
		return -1;

	*obj = strdup(ev);
	return 0;
}

/*
 *  evaluate the 'coda_info' keyword
 */
static int coda_info_evaluate(const void *s) {
	char line[MAX_DATA_LEN];
	char *keyword, *value;
	int evaluate_value = -1;

	strncpy(line, (char *)s, sizeof(line));
	keyword = strtok(line, "=");
	value = strtok(NULL, "");

	if (strcmp(keyword, "state") == 0) {
		int coda_state = 0;
		clog(LOG_DEBUG, "coda instances[%d/%d]\n", plugin_coda->update_instances, plugin_coda->total_instances);
		evaluate_value = atoi(value);
		if(plugin_coda->update_instances)
			coda_state = 1;
		return (evaluate_value == coda_state) ? MATCH : DONT_MATCH;
	}

	//all other keywords except "state" should be DONT_MATCH if not single instance case
	if(plugin_coda->update_instances != 1)
		return DONT_MATCH;

	//found the single instance
	struct LIST *coda_list = &(plugin_coda->coda_instance_list);
	struct coda_info *ci = NULL;
	struct NODE *pNode = NULL;
	int found = 0;
	struct coda_video_profile *cvf;

	LIST_FOREACH_NODE(node, coda_list) {
		pNode = node;
		ci = (struct coda_info *)node->content;
		if(ci->event == CODA_UPDATE) {
			found = 1;
			break;
		}
	}

	//must have at least one update_instance
	if(!found) {
		clog(LOG_ERR, "SHOULD never goes here, must have at least one update_instance\n");
		return DONT_MATCH;
	}

	cvf = &ci->profile;
	if (strcmp(keyword, "type") == 0) {
		int coda_type = cvf->codec_type;
		if(strcmp(value, "decode") == 0)
			evaluate_value = 0;
		if(strcmp(value, "encode") == 0)
			evaluate_value = 1;
		return (evaluate_value == coda_type) ? MATCH : DONT_MATCH;
	}

	if (strcmp(keyword, "resolution") == 0) {
		int coda_resolution = cvf->resolution;
		if(strcmp(value, "vga") == 0)
			evaluate_value = VGA;
		if(strcmp(value, "720p") == 0)
			evaluate_value = HD720P;
		if(strcmp(value, "1080p") == 0)
			evaluate_value = HD1080P;
		return (evaluate_value == coda_resolution) ? MATCH : DONT_MATCH;
	}

	//not support yet
	if (strcmp(keyword, "fps") == 0) {
		return DONT_MATCH;
	}

	if (strcmp(keyword, "stream_format") == 0) {
		int stream_format = cvf->strm_fmt;
        evaluate_value = atoi(value);
		return (evaluate_value == stream_format) ? MATCH : DONT_MATCH;
	}


	clog(LOG_ERR, "SHOULD never goes here, invalid coda objective definition\n");
	return DONT_MATCH;
}

static void coda_dummy_free (void __UNUSED__ *obj)
{
}

static PowerPolicyDaemonKeyword kw[] = {
	{
		.word = "coda_info",
		.isupdate = 0,
		.parse = &coda_info_parse,
		.evaluate = &coda_info_evaluate,
	},
	{ .word = NULL, .parse = NULL, .evaluate = NULL, .free = NULL }
};

static int coda_init(void) {
	memset(plugin_coda, 0, sizeof(_plugin_coda));
	return 0;
}

static int coda_exit(void) {
	struct LIST *coda_list = &(plugin_coda->coda_instance_list);
	list_free_sublist(coda_list, coda_list->first);
	return 0;
}

static int coda_update(void *data) {
	struct LIST *coda_list = &(plugin_coda->coda_instance_list);
	struct coda_info *update_info = (struct coda_info *)data;
	struct coda_info *ci = NULL;
	struct NODE *pNode = NULL;
	int found = 0;

	if(data == NULL)
		return -1;

	LIST_FOREACH_NODE(node, coda_list) {
		pNode = node;
		ci = (struct coda_info *)node->content;
		if(ci->tid == update_info->tid) {
			found = 1;
			break;
		}
	}

	switch(update_info->event) {
		case CODA_UPDATE:
			if(!found) {
				clog(LOG_DEBUG, "append the instance %ld\n", update_info->tid);

				pNode = node_new(update_info, sizeof(struct coda_info));
				if (pNode == NULL) {
					clog(LOG_ERR, "cannot make enough room for a new instance (%ld)\n", update_info->tid);
					return -1;
				}
				list_append(coda_list, pNode);

				mark_keyword_status("coda_info");
				plugin_coda->update_instances++;
				clog(LOG_DEBUG, "CODA_UPDATE current instances[%d]\n", plugin_coda->update_instances);
			}
			else {
				clog(LOG_ERR, "couldn't append an instance %ld, already exist!\n", update_info->tid);
			}
			break;

		case CODA_STOP:
			if(found) {
				clog(LOG_DEBUG, "remove the instance %ld\n", update_info->tid);

				plugin_coda->update_instances--;
				pNode = list_remove_node(coda_list, pNode);

				mark_keyword_status("coda_info");
				clog(LOG_DEBUG, "CODA_STOP current instances[%d]\n", plugin_coda->update_instances);
			}
			else {
				clog(LOG_ERR, "couldn't remove the instance %ld, not exist!\n", update_info->tid);
			}
			break;

		default:
			break;
	}
	return 0;
}

static PowerPolicyDaemonPlugin coda = {
	.plugin_name		= "coda",
	.plugin_tag			= TAG_CODA,
	.keywords			= kw,
	.plugin_init		= &coda_init,
	.plugin_exit		= &coda_exit,
	.plugin_update		= &coda_update,
};

PowerPolicyDaemonPlugin *create_plugin (void) {
	return &coda;
}
