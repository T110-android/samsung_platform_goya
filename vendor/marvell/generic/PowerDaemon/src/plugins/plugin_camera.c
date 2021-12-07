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

#include "plugin_tags.h"
#include "plugin_camera.h"
#include "ppd_component.h"

struct camera_global_data {
	int zoom_state;
} _plugin_camera;
static struct camera_global_data *plugin_camera = &_plugin_camera;

static int camera_init(void) {
	memset(plugin_camera, 0, sizeof(_plugin_camera));
    return 0;
}

static int camera_exit(void) {
    return 0;
}

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

static int camera_update(void *data) {
	struct camera_info_plug *info = (struct camera_info_plug *)data;

    clog(LOG_INFO, "camera_update zoom_state = %d", info->zoomEnabled);
    plugin_camera->zoom_state = info->zoomEnabled;
    mark_keyword_status("camera_info");
    return 0;
}

/*
 *  parse the keyword 'camera_info'
 */
static int camera_info_parse(const char *ev, void **obj) {
    char line[32];
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
 *  evaluate the keyword 'camera_info'
 */
static int camera_info_evaluate(const void *s) {
	char line[32];
	char *keyword, *value;
	int rule_value;

	/* avoid polluting the 's' that is const */
	strncpy(line, (char *)s, sizeof(line));
	keyword = strtok(line, "=");
	value = strtok(NULL, "");
	rule_value = atoi(value);

	if (!strcmp("zoom", keyword)) {
		return (rule_value == plugin_camera->zoom_state) ? MATCH : DONT_MATCH;
	}

	clog(LOG_ERR, "SHOULD never goes here, keyword match is done in camera_info_parse\n");
	return DONT_MATCH;
}

static PowerPolicyDaemonKeyword kw[] = {
	{
		.word = "camera_info",
		.isupdate = 0,
		.parse = &camera_info_parse,
		.evaluate = &camera_info_evaluate,
	},
	{ .word = NULL, .parse = NULL, .evaluate = NULL, .free = NULL }
};

static PowerPolicyDaemonPlugin camera = {
	.plugin_name    = "camera",
	.plugin_tag     = TAG_CAMERA,
	.keywords       = kw,
	.plugin_init    = &camera_init,
	.plugin_exit    = &camera_exit,
	.plugin_update  = &camera_update,
};

PowerPolicyDaemonPlugin *create_plugin (void) {
    return &camera;
}


