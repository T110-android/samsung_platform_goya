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

/* $Id: sjpege.c,v 1.8 2007/09/11 15:24:27 Arabidopsis Exp $ */
/* Interface routines for IJG encoding code. */
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
gs_jpeg_create_compress(stream_DCT_state * st)
{				/* Initialize error handling */
    gs_jpeg_error_setup(st);
    /* Establish the setjmp return context for gs_jpeg_error_exit to use. */
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));

    jpeg_stream_data_common_init(st->data.compress);
    jpeg_create_compress(&st->data.compress->cinfo);
    return 0;
}

int
gs_jpeg_set_defaults(stream_DCT_state * st)
{
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));
    jpeg_set_defaults(&st->data.compress->cinfo);
    return 0;
}

int
gs_jpeg_set_colorspace(stream_DCT_state * st,
		       J_COLOR_SPACE colorspace)
{
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));
    jpeg_set_colorspace(&st->data.compress->cinfo, colorspace);
    return 0;
}

int
gs_jpeg_set_linear_quality(stream_DCT_state * st,
			   int scale_factor, boolean force_baseline)
{
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));
    jpeg_set_linear_quality(&st->data.compress->cinfo,
			    scale_factor, force_baseline);
    return 0;
}

int
gs_jpeg_set_quality(stream_DCT_state * st,
		    int quality, boolean force_baseline)
{
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));
    jpeg_set_quality(&st->data.compress->cinfo,
		     quality, force_baseline);
    return 0;
}

int
gs_jpeg_start_compress(stream_DCT_state * st,
		       boolean write_all_tables)
{
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));
    jpeg_start_compress(&st->data.compress->cinfo, write_all_tables);
    return 0;
}

int
gs_jpeg_write_scanlines(stream_DCT_state * st,
			JSAMPARRAY scanlines,
			int num_lines)
{
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));
    return (int)jpeg_write_scanlines(&st->data.compress->cinfo,
				     scanlines, (JDIMENSION) num_lines);
}

int
gs_jpeg_finish_compress(stream_DCT_state * st)
{
    if (setjmp(find_jmp_buf(st->data.common->exit_jmpbuf)))
	return_error(gs_jpeg_log_error(st));
    jpeg_finish_compress(&st->data.compress->cinfo);
    return 0;
}
