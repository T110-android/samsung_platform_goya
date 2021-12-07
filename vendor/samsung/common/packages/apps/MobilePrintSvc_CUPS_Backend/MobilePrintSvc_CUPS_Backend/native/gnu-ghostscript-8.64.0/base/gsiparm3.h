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

/* $Id: gsiparm3.h,v 1.7 2007/09/11 15:24:33 Arabidopsis Exp $ */
/* ImageType 3 image parameter definition */

#ifndef gsiparm3_INCLUDED
#  define gsiparm3_INCLUDED

#include "gsiparam.h"

/*
 * See Section 4.3 of the Adobe PostScript Version 3010 Supplement
 * for a definition of ImageType 3 and 4 images.
 */

/*
 * If InterleaveType is 3, the data source for the mask is provided as an
 * additional data source *before* the data sources for the pixel data.  For
 * both InterleaveType 2 and 3, the client is responsible for always
 * providing mask data before the pixel data that it masks.  (The
 * implementation does not currently check this, but it should.)
 */
typedef enum {
    interleave_chunky = 1,
    interleave_scan_lines = 2,
    interleave_separate_source = 3
} gs_image3_interleave_type_t;
typedef struct gs_image3_s {
    gs_pixel_image_common;	/* DataDict */
    int InterleaveType;
    gs_data_image_t MaskDict;
} gs_image3_t;

#define private_st_gs_image3()	/* in gximage3.c */\
  gs_private_st_suffix_add0(st_gs_image3, gs_image3_t, "gs_image3_t",\
    image3_enum_ptrs, image3_reloc_ptrs, st_gs_pixel_image)

/*
 * Initialize an ImageType 3 image.
 */
void gs_image3_t_init(gs_image3_t * pim, gs_color_space * color_space,
		      gs_image3_interleave_type_t interleave_type);

#endif /* gsiparm3_INCLUDED */
