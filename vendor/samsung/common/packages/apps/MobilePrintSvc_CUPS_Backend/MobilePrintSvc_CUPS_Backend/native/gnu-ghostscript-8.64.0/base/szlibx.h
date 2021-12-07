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

/* $Id: szlibx.h,v 1.7 2007/09/11 15:23:45 Arabidopsis Exp $ */
/* zlib filter state definition */

#ifndef szlibx_INCLUDED
#  define szlibx_INCLUDED

/* Define an opaque type for the dynamic part of the state. */
typedef struct zlib_dynamic_state_s zlib_dynamic_state_t;

/* Define the stream state structure. */
typedef struct stream_zlib_state_s {
    stream_state_common;
    /* Parameters - compression and decompression */
    int windowBits;
    bool no_wrapper;		/* omit wrapper and checksum */
    /* Parameters - compression only */
    int level;			/* effort level */
    int method;
    int memLevel;
    int strategy;
    /* Dynamic state */
    zlib_dynamic_state_t *dynamic;
} stream_zlib_state;

/*
 * The state descriptor is public only to allow us to split up
 * the encoding and decoding filters.
 */
extern_st(st_zlib_state);
#define public_st_zlib_state()	/* in szlibc.c */\
  gs_public_st_ptrs1(st_zlib_state, stream_zlib_state,\
    "zlibEncode/Decode state", zlib_state_enum_ptrs, zlib_state_reloc_ptrs,\
    dynamic)
extern const stream_template s_zlibD_template;
extern const stream_template s_zlibE_template;

/* Shared procedures */
stream_proc_set_defaults(s_zlib_set_defaults);

#endif /* szlibx_INCLUDED */
