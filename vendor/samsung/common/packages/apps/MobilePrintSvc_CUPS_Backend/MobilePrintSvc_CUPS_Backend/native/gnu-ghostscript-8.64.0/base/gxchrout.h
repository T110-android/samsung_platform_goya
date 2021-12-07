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

/* $Id: gxchrout.h,v 1.7 2007/09/11 15:24:06 Arabidopsis Exp $ */
/* Shared procedures for outline character rendering */

#ifndef gxchrout_INCLUDED
#  define gxchrout_INCLUDED

#ifndef gs_imager_state_DEFINED
#  define gs_imager_state_DEFINED
typedef struct gs_imager_state_s gs_imager_state;
#endif

/*
 * Determine the flatness for rendering a character in an outline font.
 * This may be less than the flatness in the imager state.
 * The second argument is the default scaling for the font: 0.001 for
 * Type 1 fonts, 1.0 for TrueType fonts.
 */
double gs_char_flatness(const gs_imager_state *pis, floatp default_scale);

#endif /* gxchrout_INCLUDED */
