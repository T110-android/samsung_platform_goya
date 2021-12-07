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

/* $Id: gximage3.h,v 1.7 2007/09/11 15:24:30 Arabidopsis Exp $ */
/* ImageType 3 internal API */

#ifndef gximage3_INCLUDED
#  define gximage3_INCLUDED

#include "gsiparm3.h"
#include "gxiparam.h"

/*
 * The machinery for splitting an ImageType3 image into pixel and mask data
 * is used both for imaging per se and for writing high-level output.
 * We implement this by making the procedures for setting up the mask image
 * and clipping devices virtual.
 */

/*
 * Make the mask image device -- the device that processes mask bits.
 * The device is closed and freed at the end of processing the image.
 */
#define IMAGE3_MAKE_MID_PROC(proc)\
  int proc(gx_device **pmidev, gx_device *dev, int width, int height,\
	   gs_memory_t *mem)
typedef IMAGE3_MAKE_MID_PROC((*image3_make_mid_proc_t));

/*
 * Make the mask clip device -- the device that uses the mask image to
 * clip the (opaque) image data -- and its enumerator.
 * The device is closed and freed at the end of processing the image.
 */
#define IMAGE3_MAKE_MCDE_PROC(proc)\
  int proc(/* The initial arguments are those of begin_typed_image. */\
	       gx_device *dev,\
	   const gs_imager_state *pis,\
	   const gs_matrix *pmat,\
	   const gs_image_common_t *pic,\
	   const gs_int_rect *prect,\
	   const gx_drawing_color *pdcolor,\
	   const gx_clip_path *pcpath, gs_memory_t *mem,\
	   gx_image_enum_common_t **pinfo,\
	   /* The following arguments are added. */\
	   gx_device **pmcdev, gx_device *midev,\
	   gx_image_enum_common_t *pminfo,\
	   const gs_int_point *origin)
typedef IMAGE3_MAKE_MCDE_PROC((*image3_make_mcde_proc_t));

/*
 * Begin processing an ImageType 3 image, with the mask device creation
 * procedures as additional parameters.
 */
int gx_begin_image3_generic(gx_device * dev,
			    const gs_imager_state *pis,
			    const gs_matrix *pmat,
			    const gs_image_common_t *pic,
			    const gs_int_rect *prect,
			    const gx_drawing_color *pdcolor,
			    const gx_clip_path *pcpath, gs_memory_t *mem,
			    IMAGE3_MAKE_MID_PROC((*make_mid)),
			    IMAGE3_MAKE_MCDE_PROC((*make_mcde)),
			    gx_image_enum_common_t **pinfo);

#endif /* gximage3_INCLUDED */
