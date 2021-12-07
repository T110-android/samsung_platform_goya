/*
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

#ifndef _PLUGIN_TAG_H
#define _PLUGIN_TAG_H


enum plugin_tags {
    TAG_ANDROID,
    TAG_CODA,
    TAG_THERMAL,
    TAG_INPUT,
    TAG_DDR_HOTPLUG,
    TAG_CAMERA,
    TAG_MAX
};

struct plugin_info {
    const char *name;
    int tags;
};

static struct plugin_info plugin_infos[] = {
    {
        "android",
        TAG_ANDROID,
    },
    {
        "coda",
        TAG_CODA,
    },
    {
        "thermal",
        TAG_THERMAL,
    },
    {   "input",
        TAG_INPUT,
    },
    {
        "ddr_hotplug",
        TAG_DDR_HOTPLUG,
    },
    {
        "camera",
        TAG_CAMERA,
    },
    {
        NULL,
        TAG_MAX,
    }
};

#endif
