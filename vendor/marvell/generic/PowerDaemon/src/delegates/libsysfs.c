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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "libsysfs.h"

struct sysfs_attribute* sysfs_open_attribute(char* path) {
	struct sysfs_attribute *attr;

	if(path == NULL)
		return NULL;

	attr = malloc(sizeof(struct sysfs_attribute));
	if (NULL == attr) {
		return NULL;
	}

	memset(attr, 0, sizeof(struct sysfs_attribute));
	strncpy(attr->path, path, SYSFS_PATH_MAX);

	return attr;
}

int sysfs_read_attribute(struct sysfs_attribute *attr) {
	if(attr == NULL)
		return -1;

	attr->fd = open(attr->path,O_RDONLY);
	if (attr->fd < 0) {
		return -1;
	}

	memset(attr->value, 0, sizeof(attr->value));
	attr->len = read(attr->fd, attr->value, SYSFS_PATH_MAX);
	if (attr->len <= 0) {
		close(attr->fd);
		return -1;
	}

	if(attr->value[attr->len - 1] == '\n') {
		attr->value[attr->len - 1] = '\0';
		attr->len -= 1;
	}

	if(close(attr->fd) < 0) {
		return -1;
	}

	return 0;
}

int sysfs_write_attribute(struct sysfs_attribute *attr, char* value, unsigned int len) {
	if(attr == NULL)
		return -1;

	attr->fd = open(attr->path, O_WRONLY | O_TRUNC);
	if (attr->fd < 0) {
		return -1;
	}

	int wlen = strlen(value) < len ? strlen(value) : len;
	attr->len = write(attr->fd, value, wlen);
	if (attr->len < wlen) {
		close(attr->fd);
		return -1;
	}

	if(close(attr->fd) < 0) {
		return -1;
	}

	return 0;
}

int sysfs_close_attribute(struct sysfs_attribute *attr) {
	free(attr);
	attr = NULL;
	return 0;
}

