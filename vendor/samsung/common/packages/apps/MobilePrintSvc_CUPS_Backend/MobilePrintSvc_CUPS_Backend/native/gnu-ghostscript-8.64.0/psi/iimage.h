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

/* $Id: iimage.h,v 1.9 2009/04/19 13:54:28 Arabidopsis Exp $ */
/* Image operator entry points */
/* Requires gscspace.h, gxiparam.h */

#ifndef iimage_INCLUDED
#  define iimage_INCLUDED

/* These procedures are exported by zimage.c for other modules. */

/*
 * Define a structure for image parameters other than those defined
 * in the gs_*image*_t structure.
 */
typedef struct image_params_s {
    bool MultipleDataSources;
    ref DataSource[GS_IMAGE_MAX_COMPONENTS];
    const float *pDecode;
} image_params;

/* Extract and check parameters for an image. */
int data_image_params(const gs_memory_t *mem, 
		      const ref *op, gs_data_image_t *pim,
                      image_params *pip, bool require_DataSource,
                      int num_components, int max_bits_per_component,
                      bool has_alpha);
int pixel_image_params(i_ctx_t *i_ctx_p, const ref *op,
                       gs_pixel_image_t *pim, image_params * pip,
                       int max_bits_per_component, bool has_alpha,
                       gs_color_space *csp);

/* Exported for zimage3.c and ztrans.c */
int zimage_setup(i_ctx_t *i_ctx_p, const gs_pixel_image_t * pim,
                 const ref * sources, bool uses_color, int npop);

/* Exported for zdpnext.c */
int image1_setup(i_ctx_t * i_ctx_p, bool has_alpha);

#endif /* iimage_INCLUDED */
