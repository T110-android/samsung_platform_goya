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

/* $Id: sjbig2.h,v 1.10 2008/05/04 14:34:54 Arabidopsis Exp $ */
/* Definitions for jbig2decode filter */
/* Requires scommon.h; strimpl.h if any templates are referenced */

#ifndef sjbig2_INCLUDED
#  define sjbig2_INCLUDED

#include "stdint_.h"
#include "scommon.h"
#include <jbig2.h>

/* See zfjbig2.c for details. */
typedef struct s_jbig2_global_data_s {
	void *data;
} s_jbig2_global_data_t;

/* JBIG2Decode internal stream state */
typedef struct stream_jbig2decode_state_s
{
    stream_state_common; /* a define from scommon.h */
    s_jbig2_global_data_t *global_struct; /* to protect it from freeing by GC */
    Jbig2GlobalCtx *global_ctx;
    Jbig2Ctx *decode_ctx;
    Jbig2Image *image;
    long offset; /* offset into the image bitmap of the next byte to be returned */
    int error;
}
stream_jbig2decode_state;

#define private_st_jbig2decode_state()	\
  gs_private_st_ptrs1(st_jbig2decode_state, stream_jbig2decode_state,\
    "jbig2decode filter state", jbig2decode_state_enum_ptrs,\
     jbig2decode_state_reloc_ptrs, global_struct)
extern const stream_template s_jbig2decode_template;

/* call ins to process the JBIG2Globals parameter */
int
s_jbig2decode_make_global_data(byte *data, uint length, void **result);
int
s_jbig2decode_set_global_data(stream_state *ss, s_jbig2_global_data_t *gd);
void
s_jbig2decode_free_global_data(void *data);
	
#endif /* sjbig2_INCLUDED */
