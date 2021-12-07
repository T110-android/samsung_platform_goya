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
/* $Id: gdevmrop.h,v 1.7 2007/09/11 15:24:37 Arabidopsis Exp $ */
/* Definitions for device RasterOp implementations. */
/* Requires gxdevmem.h, gsropt.h */

#ifndef gdevmrop_INCLUDED
#  define gdevmrop_INCLUDED

/*
 * Compute the effective RasterOp for the 1-bit case,
 * taking transparency into account.
 */
gs_rop3_t gs_transparent_rop(gs_logical_operation_t lop);

#ifdef DEBUG
/* Trace a [strip_]copy_rop call. */
void trace_copy_rop(const char *cname, gx_device * dev,
		    const byte * sdata, int sourcex, uint sraster,
		    gx_bitmap_id id, const gx_color_index * scolors,
		    const gx_strip_bitmap * textures,
		    const gx_color_index * tcolors,
		    int x, int y, int width, int height,
		    int phase_x, int phase_y, gs_logical_operation_t lop);
#endif

/*
 * PostScript colors normally act as the texture for RasterOp, with a null
 * (all zeros) source.  For images with CombineWithColor = true, we need
 * a way to use the image data as the source.  We implement this with a
 * device that applies RasterOp with a specified texture to drawing
 * operations, treating the drawing color as source rather than texture.
 * The texture is a gx_device_color; it may be any type of color, even a
 * pattern.
 */
#ifndef gx_device_color_DEFINED
#  define gx_device_color_DEFINED
typedef struct gx_device_color_s gx_device_color;
#endif

#ifndef gx_device_rop_texture_DEFINED
#  define gx_device_rop_texture_DEFINED
typedef struct gx_device_rop_texture_s gx_device_rop_texture;
#endif

struct gx_device_rop_texture_s {
    gx_device_forward_common;
    gs_logical_operation_t log_op;
    gx_device_color texture;
};

#define private_st_device_rop_texture()	/* in gdevrops.c */\
  gs_private_st_composite_use_final(st_device_rop_texture,\
    gx_device_rop_texture, "gx_device_rop_texture",\
    device_rop_texture_enum_ptrs, device_rop_texture_reloc_ptrs,\
    gx_device_finalize)

/* Create a RasterOp source device. */
int gx_alloc_rop_texture_device(gx_device_rop_texture ** prsdev,
				gs_memory_t * mem,
				client_name_t cname);

/* Initialize a RasterOp source device. */
void gx_make_rop_texture_device(gx_device_rop_texture * rsdev,
				gx_device * target,
				gs_logical_operation_t lop,
				const gx_device_color * texture);

#endif /* gdevmrop_INCLUDED */
