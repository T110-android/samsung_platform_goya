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

#include "delegate_gpu.h"
#include "sysfs_gpu.h"
#include "libsysfs.h"

#define PATH_TO_GPU "/sys/devices/platform/galcore/gpu/"
#define MAX_LINE_LEN 255
//#define SYSFS_PATH_MAX 255
#define READ 1
#define WRITE 0

/* RO */
#define GPUINFO_CUR_FREQ		"scaling_cur_freq"
#define GPUINFO_MIN_FREQ		"gpuinfo_min_freq"
#define GPUINFO_MAX_FREQ		"gpuinfo_max_freq"
#define SCALING_CUR_FREQ		"scaling_cur_freq"
#define SCALING_AVAIL_FREQ		"scaling_available_freqs"
#define SCALING_AVAIL_GOVERNOR	"scaling_available_governors"

/* RW */
#define SCALING_MIN_FREQ		"scaling_min_freq"
#define SCALING_MAX_FREQ		"scaling_max_freq"
#define SCALING_GOVERNOR		"scaling_cur_governor"

#define ONDEMAND_SAMPLE_RATE	"/ondemand/sampling_rate"
#define ONDEMAND_UPTRESHOLD		"/ondemand/up_threshold"
/* WO */
#define SCALING_SET_SPEED		"/userspace/customize_rate"

int sysfs_gpu_exists(unsigned int gpu)
{
	char file[SYSFS_PATH_MAX];
	struct stat statbuf;

	snprintf(file, SYSFS_PATH_MAX, PATH_TO_GPU "gpu%u/", gpu);

	if ( stat(file, &statbuf) != 0 )
		return -ENOSYS;

	return S_ISDIR(statbuf.st_mode) ? 0 : -ENOSYS;
}

/* read only sysfs nodes */
unsigned long sysfs_gpu_get_scaling_curfreq(unsigned int gpu)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	unsigned long curfreq;

	snprintf(path, sizeof(path), PATH_TO_GPU "gpu%u/gpufreq/%s", gpu, SCALING_CUR_FREQ);
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

unsigned long sysfs_gpu_get_gpuinfo_curfreq(unsigned int gpu)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	unsigned long curfreq;

	snprintf(path, sizeof(path), PATH_TO_GPU "gpu%u/gpufreq/%s", gpu, GPUINFO_CUR_FREQ);
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

int sysfs_gpu_get_hardware_limits(unsigned int gpu,
			      unsigned long *min,
			      unsigned long *max)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	unsigned long latency;

	if ((!min) || (!max))
		return -EINVAL;

	snprintf(path, sizeof(path), PATH_TO_GPU "gpu%u/gpufreq/%s", gpu, GPUINFO_MIN_FREQ);
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

	snprintf(path, sizeof(path), PATH_TO_GPU "gpu%u/gpufreq/%s", gpu, GPUINFO_MAX_FREQ);
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


struct gpufreq_available_governors * sysfs_gpu_get_available_governors(unsigned int gpu) {
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	struct gpufreq_available_governors *first = NULL;
	struct gpufreq_available_governors *current = NULL;
	char linebuf[MAX_LINE_LEN]={0};
	unsigned int pos, i;
	unsigned int len;

	snprintf(path, sizeof(path), PATH_TO_GPU "gpu%u/gpufreq/%s", gpu, SCALING_AVAIL_GOVERNOR);
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

void sysfs_gpu_put_available_governors(struct gpufreq_available_governors *any) {
	struct gpufreq_available_governors *tmp, *next;

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

struct gpufreq_available_frequencies * sysfs_gpu_get_available_frequencies(unsigned int gpu) {
	struct gpufreq_available_frequencies *first = NULL;
	struct gpufreq_available_frequencies *current = NULL;
	char one_value[SYSFS_PATH_MAX];
	char linebuf[MAX_LINE_LEN];
	unsigned int pos, i;
	unsigned int len;
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];

	snprintf(path, sizeof(path), PATH_TO_GPU "gpu%u/gpufreq/%s", gpu, SCALING_AVAIL_FREQ);
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

void sysfs_gpu_put_available_frequencies(struct gpufreq_available_frequencies *any) {
	struct gpufreq_available_frequencies *tmp, *next;

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
int sysfs_gpu_access_scaling_governor(unsigned int gpu, char *governor, int read)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	int ret;

	if(!governor)
		return -1;

	snprintf(path, sizeof(path), PATH_TO_GPU "gpu%u/gpufreq/%s", gpu, SCALING_GOVERNOR);
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

int sysfs_gpu_access_scaling_maxfreq(unsigned int gpu, unsigned long *max_freq, int read)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	char value[SYSFS_PATH_MAX];
	int ret;

	snprintf(path, sizeof(path), PATH_TO_GPU "gpu%u/gpufreq/%s", gpu, SCALING_MAX_FREQ);
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


int sysfs_gpu_access_scaling_minfreq(unsigned int gpu, unsigned long *min_freq, int read)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	char value[SYSFS_PATH_MAX];
	int ret;

	snprintf(path, sizeof(path), PATH_TO_GPU "gpu%u/gpufreq/%s", gpu, SCALING_MIN_FREQ);
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
int sysfs_gpu_modify_scaling_setspeed(unsigned int gpu, unsigned long target_frequency)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	char freq[SYSFS_PATH_MAX];
	int ret;

	snprintf(path, sizeof(path), PATH_TO_GPU "gpu%u/gpufreq/%s", gpu, SCALING_SET_SPEED);
	attr = sysfs_open_attribute(path);
	if (!attr) {
		return -1;
	}

	snprintf(freq, SYSFS_PATH_MAX, "%lu", target_frequency);
	ret = sysfs_write_attribute(attr, freq, strlen(freq));
	sysfs_close_attribute(attr);
	return ret;
}

int sysfs_gpu_get_scaling_policy(struct gpu_state *state)
{
	int ret;

	if (!state)
		return -1;

	ret = sysfs_gpu_access_scaling_governor(state->id, state->governor, READ);
	if(ret)
		return ret;

	ret = sysfs_gpu_access_scaling_minfreq(state->id, &state->minfreq, READ);
	if(ret)
		return ret;

	ret = sysfs_gpu_access_scaling_maxfreq(state->id, &state->maxfreq, READ);
	if(ret)
		return ret;

	state->setspeed = sysfs_gpu_get_gpuinfo_curfreq(state->id);
	return 0;
}


int sysfs_gpu_set_scaling_policy(struct gpu_state *state)
{
	int ret;
    unsigned int gpu;
	unsigned long old_min;
	int write_max_first;
	char userspace_gov[] = "userspace";

	if (!state || !(state->governor[0]))
		return -1;

	if (state->maxfreq < state->minfreq)
		return -1;

    gpu = state->id;

	if (strncmp(state->governor, userspace_gov, 9) == 0) {
		if ((state->setspeed < state->minfreq)||(state->setspeed > state->maxfreq))
			return -1;

		/* set fixed frequency user define */
		ret = sysfs_gpu_access_scaling_governor(gpu, userspace_gov, WRITE);
		if (ret)
			return ret;
		/* wait for userspace governor sysfs node access right authorized */
		usleep(200000);

		ret = sysfs_gpu_access_scaling_minfreq(gpu, &(state->minfreq), WRITE);
		if(ret)
			return ret;

		ret = sysfs_gpu_access_scaling_maxfreq(gpu, &(state->maxfreq), WRITE);
		if(ret)
			return ret;

		ret = sysfs_gpu_modify_scaling_setspeed(gpu, state->setspeed);
		if (ret)
			return ret;
	}
	else {
		ret = sysfs_gpu_access_scaling_minfreq(gpu, &old_min, READ);
		if(ret)
			return ret;

		write_max_first = (old_min && (state->maxfreq < old_min) ? 0 : 1);

		if (write_max_first) {
			ret = sysfs_gpu_access_scaling_maxfreq(gpu, &state->maxfreq, WRITE);
			if(ret)
				return ret;
		}

		ret = sysfs_gpu_access_scaling_minfreq(gpu, &state->minfreq, WRITE);
		if(ret)
			return ret;

		if (!write_max_first) {
			ret = sysfs_gpu_access_scaling_maxfreq(gpu, &state->maxfreq, WRITE);
			if(ret)
				return ret;
		}

		ret = sysfs_gpu_access_scaling_governor(gpu, state->governor, WRITE);
		if(ret)
			return ret;
	}
	return 0;
}

