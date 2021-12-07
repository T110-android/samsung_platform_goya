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

/* $Id: gzacpath.h,v 1.9 2007/09/11 15:24:29 Arabidopsis Exp $ */
/* State and interface definitions for clipping path accumulator */
/* Requires gxdevice.h, gzcpath.h */

#ifndef gzacpath_INCLUDED
#  define gzacpath_INCLUDED

#include "gxcpath.h"

/*
 * Device for accumulating a rectangle list.  This device can clip
 * the list being accumulated with a clipping rectangle on the fly:
 * we use this to clip clipping paths to band boundaries when
 * rendering a band list.
 */
typedef struct gx_device_cpath_accum_s {
    gx_device_common;
    gs_memory_t *list_memory;
    gs_int_rect clip_box;
    gs_int_rect bbox;
    gx_clip_list list;
} gx_device_cpath_accum;

#define public_st_device_cpath_accum()\
  gs_public_st_complex_only(st_device_cpath_accum, gx_device_cpath_accum,\
    "gx_device_cpath_accum", 0, device_cpath_accum_enum_ptrs,\
    device_cpath_accum_reloc_ptrs, gx_device_finalize)

/* Start accumulating a clipping path. */
void gx_cpath_accum_begin(gx_device_cpath_accum * padev, gs_memory_t * mem);

/* Set the accumulator's clipping box. */
void gx_cpath_accum_set_cbox(gx_device_cpath_accum * padev,
			     const gs_fixed_rect * pbox);

/* Finish accumulating a clipping path. */
/* Note that this releases the old contents of the clipping path. */
int gx_cpath_accum_end(const gx_device_cpath_accum * padev,
		       gx_clip_path * pcpath);

/* Discard an accumulator in case of error. */
void gx_cpath_accum_discard(gx_device_cpath_accum * padev);

/* Intersect two clipping paths using an accumulator. */
int gx_cpath_intersect_path_slow(gx_clip_path *, gx_path *, int,
			gs_imager_state *, const gx_fill_params *);

int cpath_accum_fill_rect_with(gx_device_cpath_accum *pcdev, gx_device *tdev, 
			       gx_device_color *pdevc);

#endif /* gzacpath_INCLUDED */
