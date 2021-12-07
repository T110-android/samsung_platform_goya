/* Copyright (C) 2001-2007 Artifex Software, Inc.
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

/* $Id: gscicach.h,v 1.2 2007/09/11 15:24:37 Arabidopsis Exp $ */
/* A color index conversion cache. */

#ifndef gscicach_INCLUDED
#  define gscicach_INCLUDED

#include "gxdevcli.h" /* For frac31. */

#ifndef gs_color_index_cache_DEFINED
#  define gs_color_index_cache_DEFINED
typedef struct gs_color_index_cache_s gs_color_index_cache_t;
#endif

gs_color_index_cache_t *gs_color_index_cache_create(gs_memory_t *memory, 
		const gs_color_space *direct_space, gx_device *dev, gs_imager_state *pis, bool need_frac);
void gs_color_index_cache_destroy(gs_color_index_cache_t *this);

int gs_cached_color_index(gs_color_index_cache_t *this, const float *paint_values, gx_device_color *pdevc, frac31 *frac_values);

#endif /* gscicach_INCLUDED */
