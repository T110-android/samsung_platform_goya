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

/* $Id: ichar1.h,v 1.10 2007/09/11 15:24:32 Arabidopsis Exp $ */
/* Type 1 / Type 2 character rendering operator procedures */

#ifndef ichar1_INCLUDED
#  define ichar1_INCLUDED

#ifndef gs_font_type1_DEFINED
#  define gs_font_type1_DEFINED
typedef struct gs_font_type1_s gs_font_type1;
#endif

/* ---------------- Public ---------------- */

/* Render a Type 1 or Type 2 outline. */
/* This is the entire implementation of the .type1/2execchar operators. */
int charstring_execchar(i_ctx_t *i_ctx_p, int font_type_mask);

/* ---------------- Internal ---------------- */

/*
 * Get a Type 1 or Type 2 glyph outline.  This is the glyph_outline
 * procedure for the font.
 */
font_proc_glyph_outline(zchar1_glyph_outline);

/*
 * Get a glyph outline given a CharString.  The glyph_outline procedure
 * for CIDFontType 0 fonts uses this.
 */
int zcharstring_outline(gs_font_type1 *pfont, int WMode, const ref *pgref,
			const gs_glyph_data_t *pgd,
			const gs_matrix *pmat, gx_path *ppath, double sbw[4]);

int
z1_glyph_info(gs_font *font, gs_glyph glyph, const gs_matrix *pmat,
	      int members, gs_glyph_info_t *info);

int z1_glyph_info_generic(gs_font *font, gs_glyph glyph, const gs_matrix *pmat,
	      int members, gs_glyph_info_t *info, font_proc_glyph_info((*proc)), 
	      int wmode);

int z1_set_cache(i_ctx_t *i_ctx_p, gs_font_base *pbfont, ref *cnref, 
	    gs_glyph glyph, op_proc_t cont, op_proc_t *exec_cont);

#endif /* ichar1_INCLUDED */
