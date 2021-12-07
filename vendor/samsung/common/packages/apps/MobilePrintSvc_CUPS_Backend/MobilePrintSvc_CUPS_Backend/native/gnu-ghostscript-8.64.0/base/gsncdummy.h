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
/* $Id: gsncdummy.h,v 1.2 2007/09/11 15:24:37 Arabidopsis Exp $ */
/* Global definitions for the example custom color callback handling. */

#ifndef gsncdummy_INCLUDED
#define gsncdummy_INCLUDED

#if ENABLE_CUSTOM_COLOR_CALLBACK		/* Defined in src/gsnamecl.h */

/*
 * This s a list of PANTONE color names and a set of equivalent CMYK values,
 */
typedef struct pantone_list_s {
    const char *name;		/* Name of the PANTONE color */
    double c, m, y, k;		/* Equivalent CMYK values */
} pantone_list_t;


#endif		/* if ENABLE_CUSTOM_COLOR_CALLBACK */

#endif		/* if gsncdummy_INCLUDED */
