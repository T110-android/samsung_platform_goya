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

#ifndef _SYSFS_DDR_H
#define _SYSFS_DDR_H

unsigned long sysfs_ddr_get_ddrinfo_curfreq(void);
struct ddrfreq_available_governors * sysfs_ddr_get_available_governors(void);
void sysfs_ddr_put_available_governors(struct ddrfreq_available_governors *any);
struct ddrfreq_available_frequencies * sysfs_ddr_get_available_frequencies(void);
void sysfs_ddr_put_available_frequencies(struct ddrfreq_available_frequencies *any);
int sysfs_ddr_access_scaling_governor(char *governor, int read);
int sysfs_ddr_access_scaling_maxfreq(unsigned long *max_freq, int read);
int sysfs_ddr_access_scaling_minfreq(unsigned long *min_freq, int read);
int sysfs_ddr_access_polling_interval(unsigned long *interval, int read);
int sysfs_ddr_get_scaling_policy(struct ddr_state *state);
int sysfs_ddr_set_scaling_policy(struct ddr_state *state);

#endif
