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

/* $Id: ifont42.h,v 1.10 2007/09/11 15:24:18 Arabidopsis Exp $ */
/* Procedure for building a Type 42 or CIDFontType 2 font */

#ifndef ifont42_INCLUDED
#  define ifont42_INCLUDED

/* Build a type 11 (TrueType CID-keyed) or 42 (TrueType) font. */
int build_gs_TrueType_font(i_ctx_t *, os_ptr, gs_font_type42 **, font_type,
			   gs_memory_type_ptr_t, const char *, const char *,
			   build_font_options_t);

/*
 * Check a parameter for being an array of strings.  Return the parameter
 * value even if it is of the wrong type.
 */
int font_string_array_param(const gs_memory_t *mem, os_ptr, const char *, ref *);

/*
 * Get a GlyphDirectory if present.  Return 0 if present, 1 if absent,
 * or an error code.
 */
int font_GlyphDirectory_param(os_ptr, ref *);

/*
 * Get a glyph outline from GlyphDirectory.  Return an empty string if
 * the glyph is missing or out of range.
 */
int font_gdir_get_outline(const gs_memory_t *mem, const ref *, long, gs_glyph_data_t *);

/*
 * Access a given byte offset and length in an array of strings.
 * This is used for sfnts and for CIDMap.  The int argument is 2 for sfnts
 * (because of the strange behavior of odd-length strings), 1 for CIDMap.
 * Return code : 0 - success, <0 - error, 
 *               >0 - number of accessible bytes (client must cycle).
 * - mru_index/pos are used as a hint where to start searching; NULLs for no hint.
 */
int string_array_access_proc(const gs_memory_t *mem, const ref *, int, ulong, uint,
			     uint *mru_index, ulong *mru_pos, const byte **);

#endif /* ifont42_INCLUDED */
