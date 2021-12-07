/*
 *  Copyright (C) 2004-2009  Dominik Brodowski <linux@dominikbrodowski.de>
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

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "delegate_cpu.h"
#include "sysfs_cpu.h"
#include "libsysfs.h"
#include "ppd_log.h"

#define PATH_TO_CPU "/sys/devices/system/cpu/"
#define MAX_LINE_LEN 255
//#define SYSFS_PATH_MAX 255
#define READ 1
#define WRITE 0

/* RO */
#define CPUINFO_CUR_FREQ		"cpuinfo_cur_freq"
#define CPUINFO_MIN_FREQ		"cpuinfo_min_freq"
#define CPUINFO_MAX_FREQ		"cpuinfo_max_freq"
#define CPUINFO_AFFECTED_CPUS	"affected_cpus"
#define CPUINFO_RELATED_CPUS	"related_cpus"
#define SCALING_CUR_FREQ		"scaling_cur_freq"
#define SCALING_AVAIL_FREQ		"scaling_available_frequencies"
#define SCALING_AVAIL_GOVERNOR	"scaling_available_governors"


#define NUM_TRANSITIONS			"stats/total_trans"
#define CPUINFO_LATENCY			"cpuinfo_transition_latency"
/* RW */
#define SCALING_MIN_FREQ		"scaling_min_freq"
#define SCALING_MAX_FREQ		"scaling_max_freq"
#define SCALING_GOVERNOR		"scaling_governor"
#define GOV_UP_THRESHOLD		"up_threshold"
#define GOV_DOWN_THRESHOLD		"down_threshold"
#define GOV_SAMPLING_RATE		"sampling_rate"
#define ONLINE		"online"
#define LOCK		"lock"
/* WO */
#define SCALING_SET_SPEED		"scaling_setspeed"

int sysfs_wait_ready(char *path)
{
	int ret;
	int count = 20;
	do{
		ret = access(path, R_OK | W_OK);
		if(ret == 0){
			break;
		}
		usleep(10000);
		count--;
	}while(count > 0);

	return 0;
}

int sysfs_cpu_exists(unsigned int cpu)
{
	char file[SYSFS_PATH_MAX];
	struct stat statbuf;

	snprintf(file, SYSFS_PATH_MAX, PATH_TO_CPU "cpu%u/", cpu);

	if ( stat(file, &statbuf) != 0 )
		return -ENOSYS;

	return S_ISDIR(statbuf.st_mode) ? 0 : -ENOSYS;
}

/* read only sysfs nodes */
unsigned long sysfs_cpu_get_scaling_curfreq(unsigned int cpu)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	unsigned long curfreq;

	snprintf(path, sizeof(path), PATH_TO_CPU "cpu%u/cpufreq/%s", cpu, SCALING_CUR_FREQ);
	attr = sysfs_open_attribute(path);
	if (!attr) {
		return 0;
	}
	if (sysfs_read_attribute(attr)) {
		sysfs_close_attribute(attr);
		return 0;
	} else
		curfreq = strtol(attr->value, NULL, 10);
	sysfs_close_attribute(attr);
	return curfreq;
}

unsigned long sysfs_cpu_get_cpuinfo_curfreq(unsigned int cpu)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	unsigned long curfreq;

	snprintf(path, sizeof(path), PATH_TO_CPU "cpu%u/cpufreq/%s", cpu, CPUINFO_CUR_FREQ);
	attr = sysfs_open_attribute(path);
	if (!attr) {
		return 0;
	}
	if (sysfs_read_attribute(attr)) {
		sysfs_close_attribute(attr);
		return 0;
	} else
		curfreq = strtol(attr->value, NULL, 10);
	sysfs_close_attribute(attr);
	return curfreq;
}

unsigned long sysfs_cpu_get_transition_latency(unsigned int cpu)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	unsigned long latency;

	snprintf(path, sizeof(path), PATH_TO_CPU "cpu%u/cpufreq/%s", cpu, CPUINFO_LATENCY);
	attr = sysfs_open_attribute(path);
	if (!attr) {
		return 0;
	}
	if (sysfs_read_attribute(attr)) {
		sysfs_close_attribute(attr);
		return 0;
	} else
		latency = strtol(attr->value, NULL, 10);
	sysfs_close_attribute(attr);
	return latency;
}

unsigned long sysfs_cpu_get_transitions(unsigned int cpu)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	unsigned long transitions;

	snprintf(path, sizeof(path), PATH_TO_CPU "cpu%u/cpufreq/%s", cpu, NUM_TRANSITIONS);
	attr = sysfs_open_attribute(path);
	if (!attr) {
		return -1;
	}
	if (sysfs_read_attribute(attr)) {
		sysfs_close_attribute(attr);
		return -1;
	} else
		transitions = strtol(attr->value, NULL, 10);
	sysfs_close_attribute(attr);
	return transitions;
}

int sysfs_cpu_get_hardware_limits(unsigned int cpu,
			      unsigned long *min,
			      unsigned long *max)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	unsigned long latency;

	if ((!min) || (!max))
		return -EINVAL;

	snprintf(path, sizeof(path), PATH_TO_CPU "cpu%u/cpufreq/%s", cpu, CPUINFO_MIN_FREQ);
	attr = sysfs_open_attribute(path);
	if (!attr) {
		return -1;
	}
	if (sysfs_read_attribute(attr)) {
		sysfs_close_attribute(attr);
		return -1;
	} else
		*min = strtol(attr->value, NULL, 10);
	sysfs_close_attribute(attr);

	snprintf(path, sizeof(path), PATH_TO_CPU "cpu%u/cpufreq/%s", cpu, CPUINFO_MAX_FREQ);
	attr = sysfs_open_attribute(path);
	if (!attr) {
		return -1;
	}
	if (sysfs_read_attribute(attr)) {
		sysfs_close_attribute(attr);
		return -1;
	} else
		*max = strtol(attr->value, NULL, 10);
	sysfs_close_attribute(attr);

	return 0;
}


struct cpufreq_available_governors * sysfs_cpu_get_available_governors(unsigned int cpu) {
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	struct cpufreq_available_governors *first = NULL;
	struct cpufreq_available_governors *current = NULL;
	char linebuf[MAX_LINE_LEN]={0};
	unsigned int pos, i;
	unsigned int len;

	snprintf(path, sizeof(path), PATH_TO_CPU "cpu%u/cpufreq/%s", cpu, SCALING_AVAIL_GOVERNOR);
	attr = sysfs_open_attribute(path);
	if (!attr) {
		return NULL;
	}
	if (sysfs_read_attribute(attr)) {
		sysfs_close_attribute(attr);
		return NULL;
	} else
		strcpy(linebuf, attr->value);
	sysfs_close_attribute(attr);

	len = strlen(linebuf);
	pos = 0;
	for ( i = 0; i < len; i++ )
	{
		if ( linebuf[i] == ' ' || linebuf[i] == '\n' )
		{
			if ( i - pos < 2 )
				continue;
			if ( current ) {
				current->next = malloc(sizeof *current );
				if ( ! current->next )
					goto error_out;
				current = current->next;
			} else {
				first = malloc( sizeof *first );
				if ( ! first )
					goto error_out;
				current = first;
			}
			current->first = first;
			current->next = NULL;

			current->governor = malloc(i - pos + 1);
			if ( ! current->governor )
				goto error_out;

			memcpy( current->governor, linebuf + pos, i - pos);
			current->governor[i - pos] = '\0';
			pos = i + 1;
		}
	}

	return first;

 error_out:
	while ( first ) {
		current = first->next;
		if ( first->governor )
			free( first->governor );
		free( first );
		first = current;
	}
	return NULL;
}

void sysfs_cpu_put_available_governors(struct cpufreq_available_governors *any) {
	struct cpufreq_available_governors *tmp, *next;

	if (!any)
		return;

	tmp = any->first;
	while (tmp) {
		next = tmp->next;
		if (tmp->governor)
			free(tmp->governor);
		free(tmp);
		tmp = next;
	}
}

struct cpufreq_available_frequencies * sysfs_cpu_get_available_frequencies(unsigned int cpu) {
	struct cpufreq_available_frequencies *first = NULL;
	struct cpufreq_available_frequencies *current = NULL;
	char one_value[SYSFS_PATH_MAX];
	char linebuf[MAX_LINE_LEN];
	unsigned int pos, i;
	unsigned int len;
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];

	snprintf(path, sizeof(path), PATH_TO_CPU "cpu%u/cpufreq/%s", cpu, SCALING_AVAIL_FREQ);
	attr = sysfs_open_attribute(path);
	if (!attr) {
		return NULL;
	}
	if (sysfs_read_attribute(attr)) {
		sysfs_close_attribute(attr);
		return NULL;
	} else
		strcpy(linebuf, attr->value);
	sysfs_close_attribute(attr);

	len = strlen(linebuf);
	pos = 0;
	for ( i = 0; i < len; i++ )
	{
		if ( linebuf[i] == ' ' || linebuf[i] == '\n' )
		{
			if ( i - pos < 2 )
				continue;
			if ( i - pos >= SYSFS_PATH_MAX )
				goto error_out;
			if ( current ) {
				current->next = malloc(sizeof *current );
				if ( ! current->next )
					goto error_out;
				current = current->next;
			} else {
				first = malloc(sizeof *first );
				if ( ! first )
					goto error_out;
				current = first;
			}
			current->first = first;
			current->next = NULL;

			memcpy(one_value, linebuf + pos, i - pos);
			one_value[i - pos] = '\0';
			if ( sscanf(one_value, "%lu", &current->frequency) != 1 )
				goto error_out;

			pos = i + 1;
		}
	}

	return first;

 error_out:
	while ( first ) {
		current = first->next;
		free(first);
		first = current;
	}
	return NULL;
}

void sysfs_cpu_put_available_frequencies(struct cpufreq_available_frequencies *any) {
	struct cpufreq_available_frequencies *tmp, *next;

	if (!any)
		return;

	tmp = any->first;
	while (tmp) {
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
}

/* read/write sysfs nodes */
int sysfs_cpu_access_scaling_governor(unsigned int cpu, char *governor, int read)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	int ret;

	if(!governor)
		return -1;

	snprintf(path, sizeof(path), PATH_TO_CPU "cpu%u/cpufreq/%s", cpu, SCALING_GOVERNOR);
	attr = sysfs_open_attribute(path);
	if (!attr) {
		return -1;
	}

	if(read) {
		ret = sysfs_read_attribute(attr);
		if(!ret)
			strcpy(governor, attr->value);
	}
	else {
		ret = sysfs_write_attribute(attr, governor, SYSFS_PATH_MAX);
	}
	sysfs_close_attribute(attr);
	return ret;
};

int sysfs_cpu_access_governor_upthreshold(char *gov, unsigned int *up_threshold, int read)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	char value[SYSFS_PATH_MAX];
	int ret;

	if(!up_threshold || !gov)
		return 0;

	snprintf(path, sizeof(path), PATH_TO_CPU "cpufreq/%s/%s", gov, GOV_UP_THRESHOLD);

	/* wait for userspace governor sysfs node access right authorized */
	sysfs_wait_ready(path);

	attr = sysfs_open_attribute(path);
	if (!attr) {
		return -1;
	}

	if(read) {
		ret = sysfs_read_attribute(attr);
		if(!ret)
			*up_threshold = strtol(attr->value, NULL, 10);
	}
	else {
		if(!(*up_threshold)) {
			sysfs_close_attribute(attr);
			return 0;
		}
		if(*up_threshold >= 100)
			*up_threshold = 99;
		snprintf(value, SYSFS_PATH_MAX, "%d", *up_threshold);
		ret = sysfs_write_attribute(attr, value, SYSFS_PATH_MAX);
	}
	sysfs_close_attribute(attr);
	return ret;
};

int sysfs_cpu_access_governor_samplingrate(char *gov, unsigned int *sampling_rate, int read)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	char value[SYSFS_PATH_MAX];
	int ret;

	if(!sampling_rate || !gov)
		return 0;

	snprintf(path, sizeof(path), PATH_TO_CPU "cpufreq/%s/%s", gov, GOV_SAMPLING_RATE);

	/* wait for userspace governor sysfs node access right authorized */
	sysfs_wait_ready(path);

	attr = sysfs_open_attribute(path);
	if (!attr) {
		return -1;
	}

	if(read) {
		ret = sysfs_read_attribute(attr);
		if(!ret)
			*sampling_rate = strtol(attr->value, NULL, 10);
	}
	else {
		if(!(*sampling_rate)) {
			sysfs_close_attribute(attr);
			return 0;
		}
		snprintf(value, SYSFS_PATH_MAX, "%d", *sampling_rate);
		ret = sysfs_write_attribute(attr, value, SYSFS_PATH_MAX);
	}
	sysfs_close_attribute(attr);
	return ret;
};

int sysfs_cpu_access_governor_downthreshold(char *gov, unsigned int *down_threshold, int read)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	char value[SYSFS_PATH_MAX];
	int ret;

	if(!down_threshold || !gov)
		return 0;

	snprintf(path, sizeof(path), PATH_TO_CPU "cpufreq/%s/%s", gov, GOV_DOWN_THRESHOLD);

	/* wait for userspace governor sysfs node access right authorized */
	sysfs_wait_ready(path);

	attr = sysfs_open_attribute(path);
	if (!attr) {
		return -1;
	}

	if(read) {
		ret = sysfs_read_attribute(attr);
		if(!ret)
			*down_threshold = strtol(attr->value, NULL, 10);
	}
	else {
		if(!(*down_threshold)) {
			sysfs_close_attribute(attr);
			return 0;
		}
		if(*down_threshold >= 100)
			*down_threshold = 99;
		snprintf(value, SYSFS_PATH_MAX, "%d", *down_threshold);
		ret = sysfs_write_attribute(attr, value, SYSFS_PATH_MAX);
	}
	sysfs_close_attribute(attr);
	return ret;
};

int sysfs_cpu_access_scaling_maxfreq(unsigned int cpu, unsigned long *max_freq, int read)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	char value[SYSFS_PATH_MAX];
	int ret;

	snprintf(path, sizeof(path), PATH_TO_CPU "cpu%u/cpufreq/%s", cpu, SCALING_MAX_FREQ);
	attr = sysfs_open_attribute(path);
	if (!attr) {
		return -1;
	}

	if(read) {
		ret = sysfs_read_attribute(attr);
		if(!ret)
			*max_freq = strtol(attr->value, NULL, 10);
	}
	else {
		snprintf(value, SYSFS_PATH_MAX, "%lu", *max_freq);
		ret = sysfs_write_attribute(attr, value, SYSFS_PATH_MAX);
	}
	sysfs_close_attribute(attr);
	return ret;
};

int sysfs_cpu_access_online(unsigned int cpu, int *online, int read)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	char value[SYSFS_PATH_MAX];
	int ret;

	snprintf(path, sizeof(path), PATH_TO_CPU "cpu%u/%s", cpu, ONLINE);
	attr = sysfs_open_attribute(path);
	if (!attr) {
		return -1;
	}

	if(read) {
		ret = sysfs_read_attribute(attr);
		if(!ret)
			*online = strtol(attr->value, NULL, 10);
	}
	else {
		snprintf(value, SYSFS_PATH_MAX, "%d", *online);
		ret = sysfs_write_attribute(attr, value, strlen(value));
	}
	sysfs_close_attribute(attr);
	return ret;
};

int sysfs_cpu_access_hotplug_lock(int *status, int read)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX] = {0};
	char value[SYSFS_PATH_MAX] = {0};
	int ret;

	snprintf(path, sizeof(path), PATH_TO_CPU "hotplug/%s", LOCK);
	attr = sysfs_open_attribute(path);
	if (!attr) {
		return -1;
	}

	if(read) {
		ret = sysfs_read_attribute(attr);
		if(!ret)
			*status = strtol(attr->value, NULL, 10);
	}
	else {
		snprintf(value, SYSFS_PATH_MAX, "%d", *status);
		ret = sysfs_write_attribute(attr, value, strlen(value));
	}
	sysfs_close_attribute(attr);
	return ret;
};

int sysfs_cpu_access_scaling_minfreq(unsigned int cpu, unsigned long *min_freq, int read)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	char value[SYSFS_PATH_MAX];
	int ret;

	snprintf(path, sizeof(path), PATH_TO_CPU "cpu%u/cpufreq/%s", cpu, SCALING_MIN_FREQ);
	attr = sysfs_open_attribute(path);
	if (!attr) {
		return -1;
	}

	if(read) {
		ret = sysfs_read_attribute(attr);
		if(!ret)
			*min_freq = strtol(attr->value, NULL, 10);
	}
	else {
		snprintf(value, SYSFS_PATH_MAX, "%lu", *min_freq);
		ret = sysfs_write_attribute(attr, value, SYSFS_PATH_MAX);
	}
	sysfs_close_attribute(attr);
	return ret;
};

/* write only sysfs nodes */
int sysfs_cpu_modify_scaling_setspeed(unsigned int cpu, unsigned long target_frequency)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	char freq[SYSFS_PATH_MAX];
	int ret;

	snprintf(path, sizeof(path), PATH_TO_CPU "cpu%u/cpufreq/%s", cpu, SCALING_SET_SPEED);
	attr = sysfs_open_attribute(path);
	if (!attr) {
		return -1;
	}

	snprintf(freq, SYSFS_PATH_MAX, "%lu", target_frequency);
	ret = sysfs_write_attribute(attr, freq, strlen(freq));
	sysfs_close_attribute(attr);
	return ret;
}

int sysfs_cpu_get_scaling_policy(struct cpu_state *state)
{
	int ret;

	if (!state)
		return -1;

	ret = sysfs_cpu_access_scaling_governor(state->id, state->governor, READ);
	if(ret)
		return ret;

	ret = sysfs_cpu_access_scaling_minfreq(state->id, &state->minfreq, READ);
	if(ret)
		return ret;

	ret = sysfs_cpu_access_scaling_maxfreq(state->id, &state->maxfreq, READ);
	if(ret)
		return ret;

	state->setspeed = sysfs_cpu_get_cpuinfo_curfreq(state->id);

	if (strncmp(state->governor, "ondemand", 8) == 0) {
		sysfs_cpu_access_governor_upthreshold(state->governor, &(state->gov_para.up_threshold), READ);
		sysfs_cpu_access_governor_samplingrate(state->governor, &(state->gov_para.sampling_rate), READ);
	}

	if (strncmp(state->governor, "conservative", 12) == 0) {
		sysfs_cpu_access_governor_upthreshold(state->governor, &(state->gov_para.up_threshold), READ);
		sysfs_cpu_access_governor_downthreshold(state->governor, &(state->gov_para.down_threshold), READ);
	}
	return 0;
}


int sysfs_cpu_set_scaling_policy(struct cpu_state *state)
{
	int ret;
    unsigned int cpu;
	unsigned long old_min;
	int write_max_first;
	char userspace_gov[] = "userspace";

	if (!state || !(state->governor[0]))
		return -1;

	if (state->maxfreq < state->minfreq)
		return -1;

    cpu = state->id;

	if (strncmp(state->governor, userspace_gov, 9) == 0) {
		if ((state->setspeed < state->minfreq)||(state->setspeed > state->maxfreq))
			return -1;

		/* set fixed frequency user define */
		ret = sysfs_cpu_access_scaling_governor(cpu, userspace_gov, WRITE);
		if (ret)
			return ret;

		ret = sysfs_cpu_access_scaling_minfreq(cpu, &(state->minfreq), WRITE);
		if(ret)
			return ret;

		ret = sysfs_cpu_access_scaling_maxfreq(cpu, &(state->maxfreq), WRITE);
		if(ret)
			return ret;

		ret = sysfs_cpu_modify_scaling_setspeed(cpu, state->setspeed);
		if (ret)
			return ret;
	}
	else {
		ret = sysfs_cpu_access_scaling_minfreq(cpu, &old_min, READ);
		if(ret)
			return ret;

		write_max_first = (old_min && (state->maxfreq < old_min) ? 0 : 1);

		if (write_max_first) {
			ret = sysfs_cpu_access_scaling_maxfreq(cpu, &state->maxfreq, WRITE);
			if(ret)
				return ret;
		}

		ret = sysfs_cpu_access_scaling_minfreq(cpu, &state->minfreq, WRITE);
		if(ret)
			return ret;

		if (!write_max_first) {
			ret = sysfs_cpu_access_scaling_maxfreq(cpu, &state->maxfreq, WRITE);
			if(ret)
				return ret;
		}

		ret = sysfs_cpu_access_scaling_governor(cpu, state->governor, WRITE);
		if(ret)
			return ret;

		if (strncmp(state->governor, "ondemand", 8) == 0) {
			sysfs_cpu_access_governor_upthreshold(state->governor, &(state->gov_para.up_threshold), WRITE);
			sysfs_cpu_access_governor_samplingrate(state->governor, &(state->gov_para.sampling_rate), WRITE);
		}

		if (strncmp(state->governor, "conservative", 12) == 0) {
			sysfs_cpu_access_governor_upthreshold(state->governor, &(state->gov_para.up_threshold), WRITE);
			sysfs_cpu_access_governor_downthreshold(state->governor, &(state->gov_para.down_threshold), WRITE);
		}

	}

	return 0;
}

int sysfs_cpu_set_hotplug_status(int status) {
	int current_state = -1;
	int ret = sysfs_cpu_access_hotplug_lock(&current_state, READ);

	if (ret < 0) {
		clog(LOG_ERR, "read hotplug lock node failed!");
	}

	if (current_state != status) { // update only when status changes
		ret = sysfs_cpu_access_hotplug_lock(&status, WRITE);
		if (ret < 0) {
			clog(LOG_ERR, "write %d to hotplug lock node failed!", status);
		}
	}
	return ret;
}

int sysfs_cpu_get_hotplug_status(int *status) {
	int ret = sysfs_cpu_access_hotplug_lock(status, READ);

	if (ret < 0) {
		clog(LOG_ERR, "read hotplug lock node failed!");
	}
	return ret;
}

int sysfs_cpu_set_online_status(struct cpu_state *state) {
	int current_status = -1;
	int ret = sysfs_cpu_access_online(state->id, &current_status, READ);

	if (ret < 0) {
		clog(LOG_ERR, "read online node for CPU%d failed!", state->id);
	}

	if ((unsigned int)current_status != state->online) { // update only when status changes
		ret = sysfs_cpu_access_online(state->id, (int *)(&(state->online)), WRITE);
		if (ret < 0) {
			clog(LOG_ERR, "write online node for CPU%d failed!", state->id);
		}
	}
	return ret;
}

int sysfs_cpu_get_online_status(struct cpu_state *state, int *status) {
	int ret = sysfs_cpu_access_online(state->id, status, READ);

	if (ret < 0) {
		clog(LOG_ERR, "read online node for CPU%d failed!", state->id);
	}
	return ret;
}

