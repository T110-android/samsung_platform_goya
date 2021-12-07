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

/* $Id: gsnorop.c,v 1.7 2007/09/11 15:24:26 Arabidopsis Exp $ */
/* Stubs for unimplemented RasterOp */
#include "gx.h"
#include "gserrors.h"
#include "gsrop.h"
#include "gxdevcli.h"
#include "gxdevice.h"		/* for gx_default_*copy_rop prototypes */
#include "gxdevmem.h"		/* for gdevmem.h */
#include "gdevmem.h"		/* for mem_*_strip_copy_rop prototypes */
#include "gdevmrop.h"

/* Stub accessors to logical operation in graphics state. */

gs_logical_operation_t
gs_current_logical_op(const gs_state * pgs)
{
    return lop_default;
}

int
gs_set_logical_op(gs_state * pgs, gs_logical_operation_t lop)
{
    return (lop == lop_default ? 0 : gs_note_error(gs_error_rangecheck));
}

/* Stub RasterOp implementations for memory devices. */

int
mem_mono_strip_copy_rop(gx_device * dev,
	     const byte * sdata, int sourcex, uint sraster, gx_bitmap_id id,
			const gx_color_index * scolors,
	   const gx_strip_bitmap * textures, const gx_color_index * tcolors,
			int x, int y, int width, int height,
			int phase_x, int phase_y, gs_logical_operation_t lop)
{
    return_error(gs_error_rangecheck);
}

int
mem_gray_strip_copy_rop(gx_device * dev,
	     const byte * sdata, int sourcex, uint sraster, gx_bitmap_id id,
			const gx_color_index * scolors,
	   const gx_strip_bitmap * textures, const gx_color_index * tcolors,
			int x, int y, int width, int height,
			int phase_x, int phase_y, gs_logical_operation_t lop)
{
    return_error(gs_error_rangecheck);
}

int
mem_gray8_rgb24_strip_copy_rop(gx_device * dev,
	     const byte * sdata, int sourcex, uint sraster, gx_bitmap_id id,
			       const gx_color_index * scolors,
	   const gx_strip_bitmap * textures, const gx_color_index * tcolors,
			       int x, int y, int width, int height,
		       int phase_x, int phase_y, gs_logical_operation_t lop)
{
    return_error(gs_error_rangecheck);
}

/* Stub default implementations of device procedures. */

int
gx_default_copy_rop(gx_device * dev,
	     const byte * sdata, int sourcex, uint sraster, gx_bitmap_id id,
		    const gx_color_index * scolors,
	     const gx_tile_bitmap * texture, const gx_color_index * tcolors,
		    int x, int y, int width, int height,
		    int phase_x, int phase_y, gs_logical_operation_t lop)
{
    return_error(gs_error_unknownerror);	/* not implemented */
}

int
gx_default_strip_copy_rop(gx_device * dev,
	     const byte * sdata, int sourcex, uint sraster, gx_bitmap_id id,
			  const gx_color_index * scolors,
	   const gx_strip_bitmap * textures, const gx_color_index * tcolors,
			  int x, int y, int width, int height,
		       int phase_x, int phase_y, gs_logical_operation_t lop)
{
    return_error(gs_error_unknownerror);	/* not implemented */
}

int
mem_default_strip_copy_rop(gx_device * dev,
	     const byte * sdata, int sourcex, uint sraster, gx_bitmap_id id,
			  const gx_color_index * scolors,
	   const gx_strip_bitmap * textures, const gx_color_index * tcolors,
			  int x, int y, int width, int height,
		       int phase_x, int phase_y, gs_logical_operation_t lop)
{
    return_error(gs_error_unknownerror);	/* not implemented */
}

/* Stub RasterOp source devices. */

int
gx_alloc_rop_texture_device(gx_device_rop_texture ** prsdev, gs_memory_t * mem,
			    client_name_t cname)
{
    return_error(gs_error_rangecheck);
}

void
gx_make_rop_texture_device(gx_device_rop_texture * dev, gx_device * target,
	     gs_logical_operation_t log_op, const gx_device_color * texture)
{				/* Never called. */
}
