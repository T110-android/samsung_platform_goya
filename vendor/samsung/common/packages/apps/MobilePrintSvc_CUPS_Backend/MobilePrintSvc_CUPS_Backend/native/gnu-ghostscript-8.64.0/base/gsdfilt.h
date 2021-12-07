/* Copyright (C) 2001-2006 Artifex Software, Inc.
   All Rights Reserved.
  
  This file is part of GNU ghostscript

  GNU ghostscript is free software; you can redistribute it and/or
  modify it under the terms of the version 2 of the GNU General Public
  License as published by the Free Software Foundation.

  GNU ghostscript is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with
  ghostscript; see the file COPYING. If not, write to the Free Software Foundation,
  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

*/
/* $Id: gsdfilt.h,v 1.8 2007/09/11 15:24:27 Arabidopsis Exp $ */

#ifndef gsdfilt_INCLUDED
#  define gsdfilt_INCLUDED

/* The device filter stack lives in the gs_state structure. It represents
   a chained sequence of devices that filter device requests, each forwarding
   to its target. The last such target is the physical device as set by
   setpagedevice.

   There is a "shadow" gs_device_filter_stack_s object for each device in
   the chain. The stack management uses these objects to keep track of the
   chain.
*/

#ifndef gs_device_filter_stack_DEFINED
#  define gs_device_filter_stack_DEFINED
typedef struct gs_device_filter_stack_s gs_device_filter_stack_t;
#endif

#ifndef gs_device_filter_DEFINED
#  define gs_device_filter_DEFINED
typedef struct gs_device_filter_s gs_device_filter_t;
#endif

struct gs_device_filter_s {
    int (*push)(gs_device_filter_t *self, gs_memory_t *mem, gs_state *pgs,
		gx_device **pdev, gx_device *target);
    int (*prepop)(gs_device_filter_t *self, gs_memory_t *mem, gs_state *pgs,
		  gx_device *dev);
    int (*postpop)(gs_device_filter_t *self, gs_memory_t *mem, gs_state *pgs,
		   gx_device *dev);
};

extern_st(st_gs_device_filter);

/**
 * gs_push_device_filter: Push a device filter.
 * @mem: Memory for creating device filter.
 * @pgs: Graphics state.
 * @df: The device filter.
 *
 * Pushes a device filter, thereby becoming the first in the chain.
 *
 * Return value: 0 on success, or error code.
 **/
int gs_push_device_filter(gs_memory_t *mem, gs_state *pgs, gs_device_filter_t *df);

/**
 * gs_pop_device_filter: Pop a device filter.
 * @mem: Memory in which device filter was created, for freeing.
 * @pgs: Graphics state.
 *
 * Removes the topmost device filter (ie, first filter in the chain)
 * from the graphics state's device filter stack.
 *
 * Return value: 0 on success, or error code.
 **/
int gs_pop_device_filter(gs_memory_t *mem, gs_state *pgs);

/**
 * gs_clear_device_filters: Clear device filters from a graphics state.
 * @mem: Memory in which device filters were created, for freeing.
 * @pgs: Graphics state.
 *
 * Clears all device filters from the given graphics state.
 *
 * Return value: 0 on success, or error code.
 **/
int gs_clear_device_filters(gs_memory_t *mem, gs_state *pgs);


#endif /* gsdfilt_INCLUDED */
