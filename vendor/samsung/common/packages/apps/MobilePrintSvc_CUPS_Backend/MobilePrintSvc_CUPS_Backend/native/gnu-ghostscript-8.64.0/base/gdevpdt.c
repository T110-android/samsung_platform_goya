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

/* $Id: gdevpdt.c,v 1.6 2007/09/11 15:24:15 Arabidopsis Exp $ */
/* Miscellaneous external entry points for pdfwrite text */
#include "gx.h"
#include "memory_.h"
#include "gdevpdfx.h"
#include "gdevpdtx.h"
#include "gdevpdtf.h"
#include "gdevpdti.h"

/* GC descriptors */
private_st_pdf_text_data();

/* ---------------- Initialization ---------------- */

/*
 * Allocate and initialize the text data structure.
 */
pdf_text_data_t *
pdf_text_data_alloc(gs_memory_t *mem)
{
    pdf_text_data_t *ptd =
	gs_alloc_struct(mem, pdf_text_data_t, &st_pdf_text_data,
			"pdf_text_data_alloc");
    pdf_outline_fonts_t *pofs = pdf_outline_fonts_alloc(mem);
    pdf_bitmap_fonts_t *pbfs = pdf_bitmap_fonts_alloc(mem);
    pdf_text_state_t *pts = pdf_text_state_alloc(mem);

    if (pts == 0 || pbfs == 0 || pofs == 0 || ptd == 0) {
	gs_free_object(mem, pts, "pdf_text_data_alloc");
	gs_free_object(mem, pbfs, "pdf_text_data_alloc");
	gs_free_object(mem, pofs, "pdf_text_data_alloc");
	gs_free_object(mem, ptd, "pdf_text_data_alloc");
	return 0;
    }
    memset(ptd, 0, sizeof(*ptd));
    ptd->outline_fonts = pofs;
    ptd->bitmap_fonts = pbfs;
    ptd->text_state = pts;
    return ptd;
}
