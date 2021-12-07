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

/* $Id: gsutil.h,v 1.8 2007/09/11 15:24:27 Arabidopsis Exp $ */
/* Prototypes for procedures in gsutil.c */

#ifndef gsutil_INCLUDED
#  define gsutil_INCLUDED

/* ------ Unique IDs ------ */

/* Generate a block of unique IDs. */
gs_id gs_next_ids(const gs_memory_t *mem, uint count);

/* ------ Memory utilities ------ */

/* Transpose an 8 x 8 block of bits. */
/* line_size is the raster of the input data; */
/* dist is the distance between output bytes. */
/* Dot matrix printers need this. */
/* Note that with a negative dist value, */
/* this will rotate an 8 x 8 block 90 degrees counter-clockwise. */
void memflip8x8(const byte * inp, int line_size, byte * outp, int dist);

/* Get an unsigned, big-endian 32-bit value. */
ulong get_u32_msb(const byte *p);

/* ------ String utilities ------ */

/* Compare two strings, returning -1 if the first is less, */
/* 0 if they are equal, and 1 if first is greater. */
/* We can't use memcmp, because we always use unsigned characters. */
int bytes_compare(const byte * str1, uint len1,
		  const byte * str2, uint len2);

/* Test whether a string matches a pattern with wildcards. */
/* If psmp == NULL, use standard parameters: '*' = any substring, */
/* '?' = any character, '\\' quotes next character, don't ignore case. */
typedef struct string_match_params_s {
    int any_substring;		/* '*' */
    int any_char;		/* '?' */
    int quote_next;		/* '\\' */
    bool ignore_case;
    bool slash_equiv;	/* '\\' is equivalent to '/' for Windows filename matching */
} string_match_params;
extern const string_match_params string_match_params_default;
bool string_match(const byte * str, uint len,
		  const byte * pstr, uint plen,
		  const string_match_params * psmp);

/* graphical object tags */
typedef enum {
    GS_DEVICE_DOESNT_SUPPORT_TAGS = 0, /* default */
    GS_UNKNOWN_TAG = 0x1,
    GS_TEXT_TAG = 0x2,
    GS_IMAGE_TAG = 0x4,
    GS_PATH_TAG = 0x8,
    GS_UNTOUCHED_TAG = 0x10
} gs_object_tag_type_t;

/* accessors for object tags */
gs_object_tag_type_t gs_current_object_tag(void);

#include "gxstate.h"

void gs_set_object_tag(gs_state * pgs, const gs_object_tag_type_t tag);
void gs_enable_object_tagging(void);

#endif /* gsutil_INCLUDED */
