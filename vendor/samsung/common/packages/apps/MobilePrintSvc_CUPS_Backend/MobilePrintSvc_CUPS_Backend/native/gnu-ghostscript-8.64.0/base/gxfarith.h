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

/* $Id: gxfarith.h,v 1.10 2008/05/04 14:34:46 Arabidopsis Exp $ */
/* Floating point arithmetic macros for Ghostscript library */

#ifndef gxfarith_INCLUDED
#  define gxfarith_INCLUDED

#include "gxarith.h"

/*
 * Define sine and cosine functions that take angles in degrees rather than
 * radians, hit exact values at multiples of 90 degrees, and are implemented
 * efficiently on machines with slow (or no) floating point.
 */
double gs_sin_degrees(double angle);
double gs_cos_degrees(double angle);
typedef struct gs_sincos_s {
    double sin, cos;
    bool orthogonal;		/* angle is multiple of 90 degrees */
} gs_sincos_t;
void gs_sincos_degrees(double angle, gs_sincos_t * psincos);

/*
 * Define an atan2 function that returns an angle in degrees and uses
 * the PostScript quadrant rules.  Note that it may return
 * gs_error_undefinedresult.
 */
int gs_atan2_degrees(double y, double x, double *pangle);

#endif /* gxfarith_INCLUDED */
