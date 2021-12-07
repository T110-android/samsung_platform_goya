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
#include <fcntl.h>

#include "plugin_tags.h"
#include "plugin_thermal.h"
#include "ppd_component.h"

#define THERMAL_TEMPERATURE "/sys/class/thermal/thermal_zone0/temp"
#define THERMAL_TRIP_POINT0 "/sys/class/thermal/thermal_zone0/trip_point_0_temp"
#define THERMAL_TRIP_POINT1 "/sys/class/thermal/thermal_zone0/trip_point_1_temp"
#define THERMAL_TRIP_POINT2 "/sys/class/thermal/thermal_zone0/trip_point_2_temp"
//default values of thermal trip point
static int NOTICEABLE_TRIP_POINT = 85000;
static int WARNING_TRIP_POINT = 95000;
static int CRITICAL_TRIP_POINT = 105000;

struct thermal_global_data {
	int thermal_current_state;
	int thermal_current_temp;
} _plugin_thermal;
static struct thermal_global_data *plugin_thermal = &_plugin_thermal;

static int get_thermal_attr(char *sysfs_node)
{
	char value[16]={0};
	int fd, ret_temp;

	if (sysfs_node == NULL)
		return -1;

	fd = open(sysfs_node, O_RDONLY);
	if (fd == -1) {
		return -1;
	}

	read(fd, value, sizeof(value));
	close(fd);

	ret_temp = atoi(value);
	return ret_temp;
}

static int thermal_init(void) {

	memset(plugin_thermal, 0, sizeof(_plugin_thermal));

	plugin_thermal->thermal_current_temp = get_thermal_attr(THERMAL_TEMPERATURE);
	if(plugin_thermal->thermal_current_temp == -1)
		return -1;

	NOTICEABLE_TRIP_POINT = get_thermal_attr(THERMAL_TRIP_POINT0);
	if(NOTICEABLE_TRIP_POINT == -1)
		return -1;

	WARNING_TRIP_POINT = get_thermal_attr(THERMAL_TRIP_POINT1);
	if(WARNING_TRIP_POINT == -1)
		return -1;

	CRITICAL_TRIP_POINT = get_thermal_attr(THERMAL_TRIP_POINT2);
	if(CRITICAL_TRIP_POINT == -1)
		return -1;

    clog(LOG_INFO, "Trip Point 0-2 %d,%d,%d\n", NOTICEABLE_TRIP_POINT, WARNING_TRIP_POINT, CRITICAL_TRIP_POINT);

    // uevent before thermal servie started will not be processed, mark the temperature here
    if (plugin_thermal->thermal_current_temp >= CRITICAL_TRIP_POINT) {
        plugin_thermal->thermal_current_state = STATE_CRITICAL;
    } else if (plugin_thermal->thermal_current_temp >= WARNING_TRIP_POINT) {
        plugin_thermal->thermal_current_state = STATE_WARNING;
    } else if (plugin_thermal->thermal_current_temp >= NOTICEABLE_TRIP_POINT) {
        plugin_thermal->thermal_current_state = STATE_NOTICEABLE;
    } else {
        plugin_thermal->thermal_current_state = STATE_SAFE;
    }

    return 0;
}

static int thermal_exit(void) {
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

static int thermal_update(void *data) {
	struct thermal_info *info = (struct thermal_info *)data;

    plugin_thermal->thermal_current_state = info->state;
    plugin_thermal->thermal_current_temp = info->temperature;
	mark_keyword_status("thermal_state");
    return 0;
}

/*
 *  parse the keyword 'thermal_state'
 */
static int thermal_state_parse(const char *ev, void **obj) {
    char line[32];
    char *keyword;

    strncpy(line, ev, sizeof(line));
    if ((keyword = strtok(line, "=")) != NULL) {
        if (!strcmp("state", keyword)) {
            *obj = strdup(ev);
            return 0;
        }
    }

    clog(LOG_ERR, "couldn't parse %s\n", ev);
    return -1;
}

/*
 *  evaluate the keyword 'thermal_state'
 */
static int thermal_state_evaluate(const void *s) {
	char line[32];
	char *keyword, *value;
	int rule_value;

	/* avoid polluting the 's' that is const */
	strncpy(line, (char *)s, sizeof(line));
	keyword = strtok(line, "=");
	value = strtok(NULL, "");
	rule_value = atoi(value);

	if (!strcmp("state", keyword)) {
		return (rule_value == plugin_thermal->thermal_current_state) ? MATCH : DONT_MATCH;
	}

	clog(LOG_ERR, "SHOULD never goes here, keyword match is done in thermal_state_parse\n");
	return DONT_MATCH;
}

static PowerPolicyDaemonKeyword kw[] = {
	{
		.word = "thermal_state",
		.isupdate = 1,
		.parse = &thermal_state_parse,
		.evaluate = &thermal_state_evaluate,
	},
	{ .word = NULL, .parse = NULL, .evaluate = NULL, .free = NULL }
};

static PowerPolicyDaemonPlugin thermal = {
	.plugin_name    = "thermal",
	.plugin_tag     = TAG_THERMAL,
	.keywords       = kw,
	.plugin_init    = &thermal_init,
	.plugin_exit    = &thermal_exit,
	.plugin_update  = &thermal_update,
};

PowerPolicyDaemonPlugin *create_plugin (void) {
    return &thermal;
}


