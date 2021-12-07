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

/* $Id: gdevpsu.h,v 1.8 2007/09/11 15:24:06 Arabidopsis Exp $ */
/* Interface to PostScript-writing utilities */

#ifndef gdevpsu_INCLUDED
#  define gdevpsu_INCLUDED

/* Define parameters and state for PostScript-writing drivers. */
typedef struct gx_device_pswrite_common_s {
    float LanguageLevel;
    bool ProduceEPS;
    int ProcSet_version;
    long bbox_position;		/* set when writing file header */
} gx_device_pswrite_common_t;
#define PSWRITE_COMMON_PROCSET_VERSION 1000 /* for definitions in gdevpsu.c */
#define PSWRITE_COMMON_VALUES(ll, eps, psv)\
  {ll, eps, PSWRITE_COMMON_PROCSET_VERSION + (psv)}

/* ---------------- Low level ---------------- */

/* Write a 0-terminated array of strings as lines. */
int psw_print_lines(FILE *f, const char *const lines[]);

/* ---------------- File level ---------------- */

/*
 * Write the file header, up through the BeginProlog.  This must write to a
 * file, not a stream, because it may be called during finalization.
 */
int psw_begin_file_header(FILE *f, const gx_device *dev,
			   const gs_rect *pbbox,
			   gx_device_pswrite_common_t *pdpc, bool ascii);

/* End the file header.*/
int psw_end_file_header(FILE *f);

/* End the file. */
int psw_end_file(FILE *f, const gx_device *dev,
		  const gx_device_pswrite_common_t *pdpc,
		  const gs_rect *pbbox, int page_count);

/* ---------------- Page level ---------------- */

/*
 * Write the page header.
 */
int psw_write_page_header(stream *s, const gx_device *dev,
			   const gx_device_pswrite_common_t *pdpc,
			   bool do_scale, long page_ord,  int dictsize);
/*
 * Write the page trailer.  We do this directly to the file, rather than to
 * the stream, because we may have to do it during finalization.
 */
int psw_write_page_trailer(FILE *f, int num_copies, int flush);

#endif /* gdevpsu_INCLUDED */

