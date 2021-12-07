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

#include "delegate_ddr.h"
#include "sysfs_ddr.h"
#include "libsysfs.h"

#define PATH_TO_DDR "/sys/class/devfreq/devfreq-ddr/"
#define MAX_LINE_LEN 255
//#define SYSFS_PATH_MAX 255
#define READ 1
#define WRITE 0

/* RO */
#define DDRINFO_CUR_FREQ		"cur_freq"
#define SCALING_AVAIL_FREQ		"available_freqs"
#define SCALING_AVAIL_GOVERNOR	"governor"
/* RW */
#define POLLING_INTERVAL		"polling_interval"
#define SCALING_MIN_FREQ		"min_freq"
#define SCALING_MAX_FREQ		"max_freq"
#define SCALING_GOVERNOR		"governor"

/* read only sysfs nodes */
unsigned long sysfs_ddr_get_ddrinfo_curfreq()
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	unsigned long curfreq;

	snprintf(path, sizeof(path), PATH_TO_DDR "%s", DDRINFO_CUR_FREQ);
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

struct ddrfreq_available_governors * sysfs_ddr_get_available_governors() {
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	struct ddrfreq_available_governors *first = NULL;
	struct ddrfreq_available_governors *current = NULL;
	char linebuf[MAX_LINE_LEN]={0};
	unsigned int pos, i;
	unsigned int len;

	snprintf(path, sizeof(path), PATH_TO_DDR "%s", SCALING_AVAIL_GOVERNOR);
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

void sysfs_ddr_put_available_governors(struct ddrfreq_available_governors *any) {
	struct ddrfreq_available_governors *tmp, *next;

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

struct ddrfreq_available_frequencies * sysfs_ddr_get_available_frequencies() {
	struct ddrfreq_available_frequencies *first = NULL;
	struct ddrfreq_available_frequencies *current = NULL;
	char one_value[SYSFS_PATH_MAX];
	char linebuf[MAX_LINE_LEN];
	unsigned int pos, i;
	unsigned int len;
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];

	snprintf(path, sizeof(path), PATH_TO_DDR "%s", SCALING_AVAIL_FREQ);
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

void sysfs_ddr_put_available_frequencies(struct ddrfreq_available_frequencies *any) {
	struct ddrfreq_available_frequencies *tmp, *next;

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
int sysfs_ddr_access_scaling_governor(char *governor, int read)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	int ret = 0;

	if(!governor)
		return -1;

	snprintf(path, sizeof(path), PATH_TO_DDR "%s", SCALING_GOVERNOR);
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
	}
	sysfs_close_attribute(attr);
	return ret;
};

int sysfs_ddr_access_scaling_maxfreq(unsigned long *max_freq, int read)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	char value[SYSFS_PATH_MAX];
	int ret;

	snprintf(path, sizeof(path), PATH_TO_DDR "%s", SCALING_MAX_FREQ);
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

int sysfs_ddr_access_scaling_minfreq(unsigned long *min_freq, int read)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	char value[SYSFS_PATH_MAX];
	int ret;

	snprintf(path, sizeof(path), PATH_TO_DDR "%s", SCALING_MIN_FREQ);
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

int sysfs_ddr_access_polling_interval(unsigned long *interval, int read)
{
	struct sysfs_attribute *attr;
	char path[SYSFS_PATH_MAX];
	char value[SYSFS_PATH_MAX];
	int ret;

	snprintf(path, sizeof(path), PATH_TO_DDR "%s", POLLING_INTERVAL);
	attr = sysfs_open_attribute(path);
	if (!attr) {
		return -1;
	}

	if(read) {
		ret = sysfs_read_attribute(attr);
		if(!ret)
			*interval = strtol(attr->value, NULL, 10);
	}
	else {
		snprintf(value, SYSFS_PATH_MAX, "%lu", *interval);
		ret = sysfs_write_attribute(attr, value, SYSFS_PATH_MAX);
	}
	sysfs_close_attribute(attr);
	return ret;
};

/* utils function */
int sysfs_ddr_get_scaling_policy(struct ddr_state *state)
{
	int ret;

	if (!state)
		return -1;

	ret = sysfs_ddr_access_scaling_governor(state->governor, READ);
	if(ret)
		return ret;

	ret = sysfs_ddr_access_scaling_minfreq(&state->minfreq, READ);
	if(ret)
		return ret;

	ret = sysfs_ddr_access_scaling_maxfreq(&state->maxfreq, READ);
	if(ret)
		return ret;

	ret = sysfs_ddr_access_polling_interval(&state->poll_interval, READ);
	if(ret)
		return ret;

	return 0;
}


int sysfs_ddr_set_scaling_policy(struct ddr_state *state)
{
	int ret;
    unsigned long old_interval;
	unsigned long old_min;
	int write_max_first;

	if (!state)
		return -1;

	if (state->maxfreq < state->minfreq)
		return -1;

	ret = sysfs_ddr_access_polling_interval(&old_interval, READ);
	if (ret)
		return ret;
	if(old_interval != state->poll_interval) {
		ret = sysfs_ddr_access_polling_interval(&state->poll_interval, WRITE);
		if (ret)
			return ret;
	}

	ret = sysfs_ddr_access_scaling_minfreq(&old_min, READ);
	if(ret)
		return ret;

	write_max_first = (old_min && (state->maxfreq < old_min) ? 0 : 1);

	if (write_max_first) {
		ret = sysfs_ddr_access_scaling_maxfreq(&state->maxfreq, WRITE);
		if(ret)
			return ret;
	}

	ret = sysfs_ddr_access_scaling_minfreq(&state->minfreq, WRITE);
	if(ret)
		return ret;

	if (!write_max_first) {
		ret = sysfs_ddr_access_scaling_maxfreq(&state->maxfreq, WRITE);
		if(ret)
			return ret;
	}

	if(!(state->governor[0])) {
		ret = sysfs_ddr_access_scaling_governor(state->governor, WRITE);
		if(ret)
			return ret;
	}
	return 0;
}

