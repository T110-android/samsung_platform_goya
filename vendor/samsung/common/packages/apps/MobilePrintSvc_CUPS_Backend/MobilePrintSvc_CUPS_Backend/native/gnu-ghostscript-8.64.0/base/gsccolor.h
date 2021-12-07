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

/* $Id: gsccolor.h,v 1.9 2009/04/19 13:54:29 Arabidopsis Exp $ */
/* Client color structure definition */

#ifndef gsccolor_INCLUDED
#  define gsccolor_INCLUDED

#include "gsstype.h"		/* for extern_st */

/* Pattern instance, usable in color. */
#ifndef gs_pattern_instance_DEFINED
#  define gs_pattern_instance_DEFINED
typedef struct gs_pattern_instance_s gs_pattern_instance_t;
#endif

/*
 * Define the maximum number of components in a client color.
 * This must be at least 4, and should be at least 6 to accommodate
 * hexachrome DeviceN color spaces.  It is currently set to the same
 * value as Adobe CPSI can handle 
 */
#ifndef GS_CLIENT_COLOR_MAX_COMPONENTS		/* Allow override with XCFLAGS */
#  define GS_CLIENT_COLOR_MAX_COMPONENTS (8)
#endif

/* Paint (non-Pattern) colors */
typedef struct gs_paint_color_s {
    float values[GS_CLIENT_COLOR_MAX_COMPONENTS];
    /* CAUTION: The shading decomposition algorithm may allocate 
       a smaller space when a small number of color components is in use.
    */
} gs_paint_color;

/* General colors */
#ifndef gs_client_color_DEFINED
#  define gs_client_color_DEFINED
typedef struct gs_client_color_s gs_client_color;

#endif
struct gs_client_color_s {
    gs_pattern_instance_t *pattern;
    gs_paint_color paint;	/* also color for uncolored pattern */
    /* CAUTION: gs_paint_color structure must be the last field in 
       gs_client_color_s to allow allocating a smaller space when 
       a small number of color components is in use.
    */
};

extern_st(st_client_color);
#define public_st_client_color() /* in gscolor.c */\
  gs_public_st_ptrs1(st_client_color, gs_client_color, "gs_client_color",\
    client_color_enum_ptrs, client_color_reloc_ptrs, pattern)
#define st_client_color_max_ptrs 1

#endif /* gsccolor_INCLUDED */
