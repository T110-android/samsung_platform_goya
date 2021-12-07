/* Copyright (C) 2001-2007 Artifex Software, Inc.
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

/* $Id: smd5.h,v 1.9 2007/09/11 15:24:10 Arabidopsis Exp $ */
/* Definitions for MD5Encode filter */
/* Requires scommon.h; strimpl.h if any templates are referenced */

#ifndef smd5_INCLUDED
#  define smd5_INCLUDED

#include "md5.h"

/*
 * The MD5Encode filter accepts an arbitrary amount of input data, and then,
 * when closed, emits the 16-byte MD5 digest.
 */
typedef struct stream_MD5E_state_s {
    stream_state_common;
    gs_md5_state_t md5;
} stream_MD5E_state;

#define private_st_MD5E_state()	/* in smd5.c */\
  gs_private_st_simple(st_MD5E_state, stream_MD5E_state,\
    "MD5Encode state")
extern const stream_template s_MD5E_template;

stream *s_MD5E_make_stream(gs_memory_t *mem, byte *digest, int digest_size);

/*
 * The MD5Counter filter accepts an arbitrary amount of input data, 
 * and copies them to output. It also computes an MD5 digest of the data,
 * which may be gathered at any time by s_MD5C_get_digest;
 */
stream *s_MD5C_make_stream(gs_memory_t *mem, stream *strm);
/* Get a digest. Apply 'flush' before calling. */
int s_MD5C_get_digest(stream *s, byte *buf, int buf_length);

#endif /* smd5_INCLUDED */
