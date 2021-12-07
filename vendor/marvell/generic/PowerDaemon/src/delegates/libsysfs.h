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

#ifndef _SYSFS_ATTRIBUTE_H
#define _SYSFS_ATTRIBUTE_H

#define SYSFS_PATH_MAX 255

struct sysfs_attribute {
	int fd; // the file descriptor
	char path[SYSFS_PATH_MAX];
	char value[SYSFS_PATH_MAX];
	int len;
};

/*
  @Param path: the absolute path of sysfs
  @Return: the new structure related to the sysfs node
*/
struct sysfs_attribute* sysfs_open_attribute(char* path);

/*
  @Param attr: the data structure to be filled with
  @Return: 0 for success, -1 for failure
*/
int sysfs_read_attribute(struct sysfs_attribute *attr);

/*
  @Param attr:  the structure to be written
  @Param value: the data buf for writing
  @Param len: the len of data len
  @Return: 0 for success, -1 for failure
*/
int sysfs_write_attribute(struct sysfs_attribute *attr, char* value, unsigned int len);

/*
 @Param attr:  the structure to be free
 @Return: 0 for success, -1 for failure
*/
int sysfs_close_attribute(struct sysfs_attribute *attr);

#endif
