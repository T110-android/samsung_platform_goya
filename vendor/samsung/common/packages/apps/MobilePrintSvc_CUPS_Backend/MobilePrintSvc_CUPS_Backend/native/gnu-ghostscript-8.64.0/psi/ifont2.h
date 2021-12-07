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

/* $Id: ifont2.h,v 1.8 2007/09/11 15:24:21 Arabidopsis Exp $ */
/* Type 2 font utilities 2 */

#ifndef ifont2_INCLUDED
#  define ifont2_INCLUDED

/* Default value of lenIV */
#define DEFAULT_LENIV_2 (-1)

/*
 * Get the additional parameters for a Type 2 font (or FontType 2 FDArray
 * entry in a CIDFontType 0 font), beyond those common to Type 1 and Type 2
 * fonts.
 */
int type2_font_params(const_os_ptr op, charstring_font_refs_t *pfr,
		      gs_type1_data *pdata1);

#endif /* ifont2_INCLUDED */
