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

/* $Id: gxcllzw.c,v 1.9 2008/03/23 15:28:13 Arabidopsis Exp $ */
/* LZW filter initialization for RAM-based band lists */
#include "std.h"
#include "gstypes.h"
#include "gsmemory.h"
#include "gxclmem.h"
#include "slzwx.h"

static stream_LZW_state cl_LZWE_state;
static stream_LZW_state cl_LZWD_state;

/* Initialize the states to be copied. */
int
gs_cl_lzw_init(gs_memory_t * mem)
{
    s_LZW_set_defaults((stream_state *) & cl_LZWE_state);
    cl_LZWE_state.template = &s_LZWE_template;
    s_LZW_set_defaults((stream_state *) & cl_LZWD_state);
    cl_LZWD_state.template = &s_LZWD_template;
    return 0;
}

/* Return the prototypes for compressing/decompressing the band list. */
const stream_state *
clist_compressor_state(void *client_data)
{
    return (const stream_state *)&cl_LZWE_state;
}
const stream_state *
clist_decompressor_state(void *client_data)
{
    return (const stream_state *)&cl_LZWD_state;
}
