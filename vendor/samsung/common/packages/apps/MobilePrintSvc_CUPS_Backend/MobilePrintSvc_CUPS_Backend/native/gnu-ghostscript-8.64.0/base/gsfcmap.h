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

/* $Id: gsfcmap.h,v 1.8 2007/09/11 15:24:39 Arabidopsis Exp $ */
/* Public interface to CMaps */

#ifndef gsfcmap_INCLUDED
#  define gsfcmap_INCLUDED

#include "gsccode.h"

/* Define the abstract type for a CMap. */
#ifndef gs_cmap_DEFINED
#  define gs_cmap_DEFINED
typedef struct gs_cmap_s gs_cmap_t;
#endif

/* ---------------- Procedural interface ---------------- */

/*
 * Create an Identity CMap.
 */
int gs_cmap_create_identity(gs_cmap_t **ppcmap, int num_bytes, int wmode,
			    gs_memory_t *mem);

/*
 * Create an Identity CMap where each entry varies only in the lowest byte,
 * and that maps to characters rather than to CIDs.
 * (This is suitable for use as a ToUnicode CMap, for example.)
 */
int gs_cmap_create_char_identity(gs_cmap_t **ppcmap, int num_bytes,
				 int wmode, gs_memory_t *mem);

/*
 * Decode a character from a string using a CMap, updating the index.
 * Return 0 for a CID or name, N > 0 for a character code where N is the
 * number of bytes in the code, or an error.  Store the decoded bytes in
 * *pchr.  For undefined characters, set *pglyph = gs_no_glyph and return 0.
 */
int gs_cmap_decode_next(const gs_cmap_t *pcmap, const gs_const_string *str,
			uint *pindex, uint *pfidx,
			gs_char *pchr, gs_glyph *pglyph);

/*
 * Allocate and initialize a ToUnicode CMap.
 */
int gs_cmap_ToUnicode_alloc(gs_memory_t *mem, int id, int num_codes, int key_size, 
			    gs_cmap_t **ppcmap);

/*
 * Write a code pair to ToUnicode CMap.
 */
void gs_cmap_ToUnicode_add_pair(gs_cmap_t *pcmap, int code0, int code2);

#endif /* gsfcmap_INCLUDED */
