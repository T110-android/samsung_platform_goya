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

/* $Id: sjpegd.c,v 1.8 2007/09/11 15:24:41 Arabidopsis Exp $ */
/* Interface routines for IJG decoding code. */
#include "stdio_.h"
#include "string_.h"
#include "jpeglib_.h"
#include "jerror_.h"
#include "gx.h"
#include "gserrors.h"
#include "strimpl.h"
#include "sdct.h"
#include "sjpeg.h"

/*
 * Interface routines.  This layer of routines exists solely to limit
 * side-effects from using setjmp.
 */

int
gs_jpeg_create_decompress(stream_DCT_state * st)
{				/* Initialize error handling */
    gs_jpeg_error_setup(st);
    /* Establish the setjmp return context for gs_jpeg_error_exit to use. */
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));

    jpeg_stream_data_common_init(st->data.decompress);
    jpeg_create_decompress(&st->data.decompress->dinfo);
    return 0;
}

int
gs_jpeg_read_header(stream_DCT_state * st,
		    boolean require_image)
{
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));
    return jpeg_read_header(&st->data.decompress->dinfo, require_image);
}

int
gs_jpeg_start_decompress(stream_DCT_state * st)
{
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));
#if JPEG_LIB_VERSION > 55
    return (int)jpeg_start_decompress(&st->data.decompress->dinfo);
#else
    /* in IJG version 5, jpeg_start_decompress had no return value */
    jpeg_start_decompress(&st->data.decompress->dinfo);
    return 1;
#endif
}

int
gs_jpeg_read_scanlines(stream_DCT_state * st,
		       JSAMPARRAY scanlines,
		       int max_lines)
{
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));
    return (int)jpeg_read_scanlines(&st->data.decompress->dinfo,
				    scanlines, (JDIMENSION) max_lines);
}

int
gs_jpeg_finish_decompress(stream_DCT_state * st)
{
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));
    return (int)jpeg_finish_decompress(&st->data.decompress->dinfo);
}
