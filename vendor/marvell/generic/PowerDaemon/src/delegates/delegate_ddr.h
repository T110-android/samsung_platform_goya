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

#ifndef __DELEGATE_DDR_H
#define __DELEGATE_DDR_H

#include "list.h"

#define MAX_GOVERNOR_LEN 32
struct ddr_state {
	unsigned long minfreq;
	unsigned long maxfreq;
	unsigned long poll_interval;
	char governor[MAX_GOVERNOR_LEN];
};

enum ddrfreq_governor_prio {
    PRIO_DDRFREQ_ONDEMAND,
    PRIO_DDRFREQ_THROUGHPUT,
    PRIO_DDRFREQ_UNKNOWN,
    PRIO_DDRFREQ_MAX,
};

static const char *supported_governors[PRIO_DDRFREQ_MAX] = {
	[PRIO_DDRFREQ_ONDEMAND] = "simple_ondemand",
	[PRIO_DDRFREQ_THROUGHPUT] = "throughput",
	[PRIO_DDRFREQ_UNKNOWN] = "unknown",
};

struct ddrfreq_available_governors {
	char *governor;
	struct ddrfreq_available_governors *next;
	struct ddrfreq_available_governors *first;
};

struct ddrfreq_available_frequencies {
	unsigned long frequency;
	struct ddrfreq_available_frequencies *next;
	struct ddrfreq_available_frequencies *first;
};

struct ddrfreq_sys_info {
	unsigned long min;
	unsigned long max;
	struct ddrfreq_available_governors *governors;
	struct ddrfreq_available_frequencies *frequencies;
};

struct ddr_hardware_info {
	struct ddrfreq_sys_info *sys_info;
};

struct ddr_global_data {
    struct ddr_hardware_info *ddr_info;
    struct LIST ddrTicketObj;
    struct ddr_state *result;
} _delegate_ddr;

#endif

