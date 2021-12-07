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

/* $Id: szlibe.c,v 1.9 2008/03/23 15:27:41 Arabidopsis Exp $ */
/* zlib encoding (compression) filter stream */
#include "std.h"
#include "strimpl.h"
#include "szlibxx.h"

/* Initialize the filter. */
static int
s_zlibE_init(stream_state * st)
{
    stream_zlib_state *const ss = (stream_zlib_state *)st;
    int code = s_zlib_alloc_dynamic_state(ss);

    if (code < 0)
	return ERRC;	/****** WRONG ******/
    if (deflateInit2(&ss->dynamic->zstate, ss->level, ss->method,
		     (ss->no_wrapper ? -ss->windowBits : ss->windowBits),
		     ss->memLevel, ss->strategy) != Z_OK)
	return ERRC;	/****** WRONG ******/
    return 0;
}

/* Reinitialize the filter. */
static int
s_zlibE_reset(stream_state * st)
{
    stream_zlib_state *const ss = (stream_zlib_state *)st;

    if (deflateReset(&ss->dynamic->zstate) != Z_OK)
	return ERRC;	/****** WRONG ******/
    return 0;
}

/* Process a buffer */
static int
s_zlibE_process(stream_state * st, stream_cursor_read * pr,
		stream_cursor_write * pw, bool last)
{
    stream_zlib_state *const ss = (stream_zlib_state *)st;
    z_stream *zs = &ss->dynamic->zstate;
    const byte *p = pr->ptr;
    int status;

    /* Detect no input or full output so that we don't get */
    /* a Z_BUF_ERROR return. */
    if (pw->ptr == pw->limit)
	return 1;
    if (p == pr->limit && !last)
	return 0;
    zs->next_in = (Bytef *)p + 1;
    zs->avail_in = pr->limit - p;
    zs->next_out = pw->ptr + 1;
    zs->avail_out = pw->limit - pw->ptr;
    status = deflate(zs, (last ? Z_FINISH : Z_NO_FLUSH));
    pr->ptr = zs->next_in - 1;
    pw->ptr = zs->next_out - 1;
    switch (status) {
	case Z_OK:
	    return (pw->ptr == pw->limit ? 1 : pr->ptr > p && !last ? 0 : 1);
	case Z_STREAM_END:
	    return (last && pr->ptr == pr->limit ? 0 : ERRC);
	default:
	    return ERRC;
    }
}

/* Release the stream */
static void
s_zlibE_release(stream_state * st)
{
    stream_zlib_state *const ss = (stream_zlib_state *)st;

    deflateEnd(&ss->dynamic->zstate);
    s_zlib_free_dynamic_state(ss);
}

/* Stream template */
const stream_template s_zlibE_template = {
    &st_zlib_state, s_zlibE_init, s_zlibE_process, 1, 1, s_zlibE_release,
    s_zlib_set_defaults, s_zlibE_reset
};
