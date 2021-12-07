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
/* $Id: gdevdsp2.h,v 1.8 2007/09/11 15:24:07 Arabidopsis Exp $ */
/* gdevdsp2.c */

#ifndef gdevdsp2_INCLUDED
#  define gdevdsp2_INCLUDED

typedef struct gx_device_display_s gx_device_display;

#define gx_device_display_common\
	gx_device_memory *mdev;\
	display_callback *callback;\
	void *pHandle;\
	int nFormat;\
	void *pBitmap;\
	unsigned long ulBitmapSize;\
	int HWResolution_set;\
        gs_devn_params devn_params;\
        equivalent_cmyk_color_params equiv_cmyk_colors

/* The device descriptor */
struct gx_device_display_s {
    gx_device_common;
    gx_device_display_common;
};

extern_st(st_device_display);
#define public_st_device_display()	/* in gdevdsp.c */\
  gs_public_st_composite_use_final(st_device_display, gx_device_display,\
    "gx_device_display", display_enum_ptrs, display_reloc_ptrs,\
    gx_device_finalize)

#endif /* gdevdsp2_INCLUDED */
