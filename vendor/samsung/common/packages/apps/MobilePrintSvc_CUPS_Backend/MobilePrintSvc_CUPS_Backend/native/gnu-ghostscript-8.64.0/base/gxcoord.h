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

/* $Id: gxcoord.h,v 1.7 2007/09/11 15:23:56 Arabidopsis Exp $ */
/* Internal graphics state CTM procedures */
/* Requires gxmatrix.h and gzstate.h */

#ifndef gxcoord_INCLUDED
#  define gxcoord_INCLUDED

#include "gscoord.h"

/* Set the translation to a fixed value, and translate any existing path. */
/* Used by gschar.c to prepare for a BuildChar or BuildGlyph procedure. */
int gx_translate_to_fixed(gs_state *, fixed, fixed);

/* Scale the CTM and character matrix for oversampling. */
int gx_scale_char_matrix(gs_state *, int, int);

/* Compute the coefficients for fast fixed-point distance transformations */
/* from a transformation matrix. */
int gx_matrix_to_fixed_coeff(const gs_matrix *, fixed_coeff *, int);

#endif /* gxcoord_INCLUDED */
