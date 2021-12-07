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

/* $Id: sjbig2_luratech.h,v 1.6 2008/05/04 14:34:54 Arabidopsis Exp $ */
/* Definitions for jbig2decode filter - Luratech version */
/* Requires scommon.h; strimpl.h if any templates are referenced */

#ifndef sjbig2_luratech_INCLUDED
#  define sjbig2_luratech_INCLUDED

#include "scommon.h"
#include <ldf_jb2.h>

/* See zfjbig2.c for details. */
typedef struct s_jbig2_global_data_s {
	void *data;
} s_jbig2_global_data_t;

/* JBIG2Decode internal stream state */
typedef struct stream_jbig2decode_state_s
{
    stream_state_common;	/* inherit base object from scommon.h */
    JB2_Handle_Document doc;	/* Luratech JBIG2 codec context */
    s_jbig2_global_data_t *global_struct; /* to protect it from freeing by GC */
    unsigned char *global_data;
    unsigned long global_size;
    unsigned char *inbuf;  /* compressed image data */
    unsigned long insize, infill;
    unsigned char *image;  /* decoded image data */
    unsigned long width, height;
    unsigned long row, stride;
    unsigned long offset;  /* next output byte to be returned */
    JB2_Error error;
}
stream_jbig2decode_state;

#define private_st_jbig2decode_state()	\
  gs_private_st_ptrs1(st_jbig2decode_state, stream_jbig2decode_state,\
    "jbig2decode filter state", jbig2decode_state_enum_ptrs,\
     jbig2decode_state_reloc_ptrs, global_struct)
extern const stream_template s_jbig2decode_template;

/* call in to process the JBIG2Globals parameter */
int
s_jbig2decode_make_global_data(byte *data, uint size, void **result);
int
s_jbig2decode_set_global_data(stream_state *ss, s_jbig2_global_data_t *gs);
void
s_jbig2decode_free_global_data(void *data);


/* JBIG2 encoder internal state */
typedef struct stream_jbig2encode_state_s
{
    stream_state_common;	/* inherit base object from scommon.h */
    JB2_Handle_Compress cmp;	/* compression library context */
    JB2_Handle_Document doc;
    unsigned long width, height;
    unsigned long stride;	/* line length in bytes */
    unsigned char *line;	/* single line working buffer */
    unsigned long linefill;	/* bytes in the working buffer */
    unsigned char *outbuf;	/* output data buffer */
    unsigned long outsize;	/* bytes available in the buffer */ 
    unsigned long outfill;	/* bytes written to the buffer */ 
    unsigned long offset;	/* bytes written from the buffer */ 

} stream_jbig2encode_state;

#define private_st_jbig2encode_state()	\
  gs_private_st_simple(st_jbig2encode_state, stream_jbig2encode_state,\
    "jbig2encode filter state")

extern const stream_template s_jbig2encode_template;

#endif /* sjbig2_luratech_INCLUDED */
