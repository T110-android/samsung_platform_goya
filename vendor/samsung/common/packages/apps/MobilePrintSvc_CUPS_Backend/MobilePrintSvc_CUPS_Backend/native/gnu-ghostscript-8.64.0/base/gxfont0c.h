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

/* $Id: gxfont0c.h,v 1.6 2007/09/11 15:24:40 Arabidopsis Exp $ */
/* Interface to gsfont0c.c */

#ifndef gxfont0c_INCLUDED
#  define gxfont0c_INCLUDED

#include "gxfont0.h"
#include "gxfcid.h"

/* Exported by gsfont0c.c */

/*
 * Create a Type 0 font wrapper for a CIDFont.
 * psmat == NULL means use the identity matrix.
 */
int gs_font_type0_from_cidfont(gs_font_type0 **ppfont0, gs_font *font,
			       int wmode, const gs_matrix *psmat,
			       gs_memory_t *mem);

/*
 * Create a Type 0 font wrapper for a Type 42 font (converted to a Type 2
 * CIDFont), optionally using the TrueType cmap as the CMap.
 * See gs_cmap_from_type42_cmap below for details.
 */
int gs_font_type0_from_type42(gs_font_type0 **ppfont0, gs_font_type42 *pfont42,
			      int wmode, bool use_cmap, gs_memory_t *mem);

/* Exported by gsfcid2.c */

/*
 * Create a Type 2 CIDFont from a Type 42 font.
 */
int gs_font_cid2_from_type42(gs_font_cid2 **ppfcid, gs_font_type42 *pfont42,
			     int wmode, gs_memory_t *mem);

/*
 * Create a CMap from a TrueType cmap (Platform 3, Encoding 1, Format 4 only).
 * The resulting CMap is marked as being Unicode-based.
 */
int gs_cmap_from_type42_cmap(gs_cmap_t **ppcmap, gs_font_type42 *pfont42,
			     int wmode, gs_memory_t *mem);

#endif /* gxfont0c_INCLUDED */
