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

/* $Id: gxcvalue.h,v 1.7 2007/09/11 15:24:39 Arabidopsis Exp $ */
/* Definition of device color values */

#ifndef gxcvalue_INCLUDED
#  define gxcvalue_INCLUDED

/* Define the type for gray or RGB values at the driver interface. */
typedef unsigned short gx_color_value;

#define arch_sizeof_gx_color_value arch_sizeof_short
/* We might use less than the full range someday. */
/* ...bits must lie between 8 and 16. */
#define gx_color_value_bits (sizeof(gx_color_value) * 8)
#define gx_max_color_value ((gx_color_value)((1L << gx_color_value_bits) - 1))
#define gx_color_value_to_byte(cv)\
  ((cv) >> (gx_color_value_bits - 8))
#define gx_color_value_from_byte(cb)\
  (((cb) << (gx_color_value_bits - 8)) + ((cb) >> (16 - gx_color_value_bits)))

/* Convert between gx_color_values and fracs. */
#define frac2cv(fr) frac2ushort(fr)
#define cv2frac(cv) ushort2frac(cv)

#endif /* gxcvalue_INCLUDED */
