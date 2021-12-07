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

#ifndef _SYSFS_GPU_H
#define _SYSFS_GPU_H

int sysfs_gpu_exists(unsigned int gpu);
unsigned long sysfs_gpu_get_scaling_curfreq(unsigned int gpu);
unsigned long sysfs_gpu_get_gpuinfo_curfreq(unsigned int gpu);
int sysfs_gpu_get_hardware_limits(unsigned int gpu, unsigned long *min, unsigned long *max);
struct gpufreq_available_governors * sysfs_gpu_get_available_governors(unsigned int gpu);
void sysfs_gpu_put_available_governors(struct gpufreq_available_governors *any);
struct gpufreq_available_frequencies * sysfs_gpu_get_available_frequencies(unsigned int gpu);
void sysfs_gpu_put_available_frequencies(struct gpufreq_available_frequencies *any);
int sysfs_gpu_access_scaling_governor(unsigned int gpu, char *governor, int read);
int sysfs_gpu_access_scaling_maxfreq(unsigned int gpu, unsigned long *max_freq, int read);
int sysfs_gpu_access_scaling_minfreq(unsigned int gpu, unsigned long *min_freq, int read);
int sysfs_gpu_modify_scaling_setspeed(unsigned int gpu, unsigned long target_frequency);
int sysfs_gpu_get_scaling_policy(struct gpu_state *state);
int sysfs_gpu_set_scaling_policy(struct gpu_state *state);

#endif
