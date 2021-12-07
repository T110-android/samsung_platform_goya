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

/* $Id: sbhc.h,v 1.7 2007/09/11 15:23:53 Arabidopsis Exp $ */
/* Definitions for BoundedHuffman filters */
/* Requires strimpl.h */

#ifndef sbhc_INCLUDED
#  define sbhc_INCLUDED

#include "shc.h"

/*
 * The BoundedHuffman filters extend the basic Huffman coding model by
 * providing the ability to encode runs of zeros as a single data item,
 * and by providing an end-of-data (EOD) marker.
 */
#define max_zero_run 100

/* Common state */
#define stream_BHC_state_common\
	stream_hc_state_common;\
	hc_definition definition;\
		/* The client sets the following before initialization. */\
	bool EndOfData;\
	uint EncodeZeroRuns;\
		/* The following are updated dynamically. */\
	int zeros		/* # of zeros scanned or left to output */
typedef struct stream_BHC_state_s {
    stream_BHC_state_common;
} stream_BHC_state;

/* BoundedHuffmanEncode */
typedef struct stream_BHCE_state_s {
    stream_BHC_state_common;
    hce_table encode;
} stream_BHCE_state;

#define private_st_BHCE_state()	/* in sbhc.c */\
  gs_private_st_ptrs3(st_BHCE_state, stream_BHCE_state,\
    "BoundedHuffmanEncode state", bhce_enum_ptrs, bhce_reloc_ptrs,\
    definition.counts, definition.values, encode.codes)
extern const stream_template s_BHCE_template;

#define s_bhce_init_inline(ss)\
  (s_hce_init_inline(ss), (ss)->zeros = 0)

/* BoundedHuffmanDecode */
typedef struct stream_BHCD_state_s {
    stream_BHC_state_common;
    hcd_table decode;
} stream_BHCD_state;

#define private_st_BHCD_state()	/* in sbhc.c */\
  gs_private_st_ptrs3(st_BHCD_state, stream_BHCD_state,\
    "BoundedHuffmanDecode state", bhcd_enum_ptrs, bhcd_reloc_ptrs,\
    definition.counts, definition.values, decode.codes)
extern const stream_template s_BHCD_template;

#define s_bhcd_init_inline(ss)\
  (s_hcd_init_inline(ss), (ss)->zeros = 0)

/* Declare variables that hold the decoder state. */
#define bhcd_declare_state\
	hcd_declare_state;\
	int zeros

/* Load the state from the stream. */
/* Free variables: pr, ss, p, rlimit, bits, bits_left, zeros. */
#define bhcd_load_state()\
	hcd_load_state(), zeros = ss->zeros

/* Store the state back in the stream. */
/* Free variables: pr, ss, p, bits, bits_left, zeros. */
#define bhcd_store_state()\
	hcd_store_state(), ss->zeros = zeros

#endif /* sbhc_INCLUDED */
