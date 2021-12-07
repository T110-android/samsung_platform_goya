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

/* $Id: gdevpdtw.h,v 1.9 2007/09/11 15:23:50 Arabidopsis Exp $ */
/* Font and CMap resource writing API for pdfwrite */

#ifndef gdevpdtw_INCLUDED
#  define gdevpdtw_INCLUDED

/*
 * The procedures declared here are called only from gdevpdtf.c: they are
 * not intended to be called from anywhere else.
 */

/* ---------------- Font resource writing ---------------- */

/*
 * Each of these procedures is referenced only from a single place in
 * gdevpdtf.c.  Their prototype and functionality must match the definition
 * of pdf_font_write_contents_proc_t in gdevpdtf.h.
 */
int
  pdf_write_contents_type0(gx_device_pdf *pdev, pdf_font_resource_t *pdfont),
  pdf_finish_write_contents_type3(gx_device_pdf *pdev,
				  pdf_font_resource_t *pdfont),
  pdf_write_contents_std(gx_device_pdf *pdev, pdf_font_resource_t *pdfont),
  pdf_write_contents_simple(gx_device_pdf *pdev, pdf_font_resource_t *pdfont),
  pdf_write_contents_cid0(gx_device_pdf *pdev, pdf_font_resource_t *pdfont),
  pdf_write_contents_cid2(gx_device_pdf *pdev, pdf_font_resource_t *pdfont),
  pdf_different_encoding_index(const pdf_font_resource_t *pdfont, int ch0),
  pdf_write_encoding(gx_device_pdf *pdev, const pdf_font_resource_t *pdfont, long id, int ch),
  pdf_write_encoding_ref(gx_device_pdf *pdev, const pdf_font_resource_t *pdfont, long id);


/* ---------------- CMap resource writing ---------------- */

#ifndef gs_cid_system_info_DEFINED
#  define gs_cid_system_info_DEFINED
typedef struct gs_cid_system_info_s gs_cid_system_info_t;
#endif
#ifndef gs_cmap_DEFINED
#  define gs_cmap_DEFINED
typedef struct gs_cmap_s gs_cmap_t;
#endif

/*
 * Write the CIDSystemInfo for a CIDFont or a CMap.
 */
int pdf_write_cid_system_info(gx_device_pdf *pdev,
			      const gs_cid_system_info_t *pcidsi, gs_id object_id);

/* Write CIDSystemInfo */
int pdf_write_cid_systemInfo_separate(gx_device_pdf *pdev, 
		              const gs_cid_system_info_t *pcidsi, long *id);



/*
 * Write a CMap resource.  We pass the CMap object as well as the resource,
 * because we write CMaps when they are created.
 */
int pdf_write_cmap(gx_device_pdf *pdev, const gs_cmap_t *pcmap,
		   pdf_resource_t **ppres, int font_index_only);

/* 
 * Write OneByteIdentityH CMap. 
 */
int pdf_write_OneByteIdentityH(gx_device_pdf *pdev);

#endif /* gdevpdtw_INCLUDED */
