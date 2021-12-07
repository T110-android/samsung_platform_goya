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

#ifndef __DELEGATE_CPU_H
#define __DELEGATE_CPU_H

#include "list.h"

#define MAX_GOVERNOR_LEN 32
struct governor_parameter {
	unsigned int up_threshold;
	unsigned int down_threshold;
	unsigned int sampling_rate;
	//may add more here
};

struct cpu_state {
	unsigned int id;
	unsigned long minfreq;
	unsigned long maxfreq;
	unsigned long setspeed;
	unsigned int online;
	char governor[MAX_GOVERNOR_LEN];
	struct governor_parameter gov_para;
};

enum cpufreq_governor_prio {
	PRIO_CPUFREQ_ONDEMAND,
	PRIO_CPUFREQ_INTERACTIVE,
	PRIO_CPUFREQ_POWERSAVE,
	PRIO_CPUFREQ_CONSERVATIVE,
	PRIO_CPUFREQ_PERFORMANCE,
	PRIO_CPUFREQ_USERSPACE,
	PRIO_CPUFREQ_UNKNOWN,
	PRIO_CPUFREQ_MAX,
};

static const char *supported_governors[PRIO_CPUFREQ_MAX] = {
	[PRIO_CPUFREQ_POWERSAVE] = "powersave",
	[PRIO_CPUFREQ_CONSERVATIVE] = "conservative",
	[PRIO_CPUFREQ_ONDEMAND] = "ondemand",
	[PRIO_CPUFREQ_INTERACTIVE] = "interactive",
	[PRIO_CPUFREQ_PERFORMANCE] = "performance",
	[PRIO_CPUFREQ_USERSPACE] = "userspace",
	[PRIO_CPUFREQ_UNKNOWN] = "unknown",
};

//////////////////////////////////////
struct cpufreq_available_governors {
	char *governor;
	struct cpufreq_available_governors *next;
	struct cpufreq_available_governors *first;
};

struct cpufreq_available_frequencies {
	unsigned long frequency;
	struct cpufreq_available_frequencies *next;
	struct cpufreq_available_frequencies *first;
};

struct cpufreq_sys_info {
	unsigned long min;
	unsigned long max;
	struct cpufreq_available_governors *governors;
	struct cpufreq_available_frequencies *frequencies;
};

struct cpu_hardware_info {
	unsigned int cpus;
	struct cpufreq_sys_info *sys_info;
};

struct cpu_global_data {
	struct cpu_hardware_info *cpu_info;
	struct LIST cpuTicketObj;
	struct cpu_state *result;
} _delegate_cpu;

#endif

