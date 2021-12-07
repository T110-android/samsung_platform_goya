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

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <cutils/properties.h>

#include "plugin_tags.h"
#include "plugin_ddr_hotplug.h"
#include "ppd_component.h"

struct ddr_hotplug_global_data {
	int ddr_hotplug_state;
} _plugin_ddr_hotplug;
static struct ddr_hotplug_global_data *plugin_ddr_hotplug = &_plugin_ddr_hotplug;

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

static int ddr_hotplug_init(void) {
	plugin_ddr_hotplug->ddr_hotplug_state = DDR_ONLINE;
    return 0;
}

static int ddr_hotplug_exit(void) {
    return 0;
}

static int ddr_hotplug_update(void *data) {
	struct ddr_hotplug_info *info = (struct ddr_hotplug_info*)data;
    plugin_ddr_hotplug->ddr_hotplug_state = info->state;
	mark_keyword_status("ddr_hotplug_state");
    return 0;
}

/*
 *  parse the keyword 'ddr_hotplug_state'
 */
static int ddr_hotplug_state_parse(const char *ev, void **obj) {
    char line[32];
    char *keyword;

    strncpy(line, ev, sizeof(line));
    if ((keyword = strtok(line, "=")) != NULL) {
        if (!strcmp("hotplug", keyword)) {
            *obj = strdup(ev);
            return 0;
        }
    }

    clog(LOG_ERR, "couldn't parse %s\n", ev);
    return -1;
}

/*
 *  evaluate the keyword 'ddr_hotplug_state'
 */
static int ddr_hotplug_state_evaluate(const void *s) {
	char line[32];
	char *keyword, *value;
	int rule_value;

	/* avoid polluting the 's' that is const */
	strncpy(line, (char *)s, sizeof(line));
	keyword = strtok(line, "=");
	value = strtok(NULL, "");
	rule_value = atoi(value);

	if (!strcmp("hotplug", keyword)) {
		return (rule_value == plugin_ddr_hotplug->ddr_hotplug_state) ? MATCH : DONT_MATCH;
	}

	clog(LOG_ERR, "SHOULD never goes here, keyword match is done in thermal_state_parse\n");
	return DONT_MATCH;
}

static PowerPolicyDaemonKeyword kw[] = {
	{
		.word = "ddr_hotplug_state",
		.isupdate = 0,
		.parse = &ddr_hotplug_state_parse,
		.evaluate = &ddr_hotplug_state_evaluate,
	},
	{ .word = NULL, .parse = NULL, .evaluate = NULL, .free = NULL }
};

static PowerPolicyDaemonPlugin ddr_hotplug = {
	.plugin_name    = "ddr_hotplug",
	.plugin_tag     = TAG_DDR_HOTPLUG,
	.keywords       = kw,
	.plugin_init    = &ddr_hotplug_init,
	.plugin_exit    = &ddr_hotplug_exit,
	.plugin_update  = &ddr_hotplug_update,
};

PowerPolicyDaemonPlugin *create_plugin (void) {
    return &ddr_hotplug;
}


