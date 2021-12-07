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

#ifndef __DELEGATE_GPU_H
#define __DELEGATE_GPU_H

#include "list.h"

#define MAX_GOVERNOR_LEN 32
struct gpu_state {
	unsigned int id;
	unsigned long minfreq;
	unsigned long maxfreq;
	unsigned long setspeed;
	char governor[MAX_GOVERNOR_LEN];
};

enum gpufreq_governor_prio {
    PRIO_GPUFREQ_POWERSAVE,
    PRIO_GPUFREQ_ONDEMAND,
    PRIO_GPUFREQ_PERFORMANCE,
    PRIO_GPUFREQ_USERSPACE,
    PRIO_GPUFREQ_UNKNOWN,
    PRIO_GPUFREQ_MAX,
};

static const char *supported_governors[PRIO_GPUFREQ_MAX] = {
    [PRIO_GPUFREQ_POWERSAVE] = "powersave",
    [PRIO_GPUFREQ_ONDEMAND] = "ondemand",
    [PRIO_GPUFREQ_PERFORMANCE] = "performance",
    [PRIO_GPUFREQ_USERSPACE] = "userspace",
    [PRIO_GPUFREQ_UNKNOWN] = "unknown",
};

//////////////////////////////////////
struct gpufreq_available_governors {
	char *governor;
	struct gpufreq_available_governors *next;
	struct gpufreq_available_governors *first;
};

struct gpufreq_available_frequencies {
	unsigned long frequency;
	struct gpufreq_available_frequencies *next;
	struct gpufreq_available_frequencies *first;
};

struct gpufreq_sys_info {
	unsigned long min;
	unsigned long max;
	struct gpufreq_available_governors *governors;
	struct gpufreq_available_frequencies *frequencies;
};

struct gpu_hardware_info {
	unsigned int gpus;
	struct gpufreq_sys_info *sys_info;
};

struct gpu_global_data {
    struct gpu_hardware_info *gpu_info;
    struct LIST gpuTicketObj;
    struct gpu_state *result;
} _delegate_gpu;

#endif

