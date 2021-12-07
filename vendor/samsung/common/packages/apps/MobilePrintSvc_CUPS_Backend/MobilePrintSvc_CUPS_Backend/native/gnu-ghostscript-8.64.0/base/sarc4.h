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

/* $Id: sarc4.h,v 1.8 2007/09/11 15:24:11 Arabidopsis Exp $ */
/* Definitions for Arcfour cipher and filter */
/* Requires scommon.h; strimpl.h if any templates are referenced */

#ifndef sarc4_INCLUDED
#  define sarc4_INCLUDED

#include "scommon.h"

/* Arcfour is a symmetric cipher whose state is maintained
 * in two indices into an accompanying 8x8 S box. this will
 * typically be allocated on the stack, and so has no memory
 * management associated.
 */
struct stream_arcfour_state_s
{
    stream_state_common;	/* a define from scommon.h */
    unsigned int x, y;
    unsigned char S[256];
};

#ifndef stream_arcfour_state_DEFINED
#define stream_arcfour_state_DEFINED
typedef struct stream_arcfour_state_s stream_arcfour_state;
#endif

int s_arcfour_set_key(stream_arcfour_state * state, const unsigned char *key,
		      int keylength);

#define private_st_arcfour_state()	/* used in sarc4.c */\
  gs_private_st_simple(st_arcfour_state, stream_arcfour_state,\
    "Arcfour filter state")
extern const stream_template s_arcfour_template;

/* (de)crypt a section of text in a buffer -- the procedure is the same
 * in each direction. see strimpl.h for return codes.
 */
int s_arcfour_process_buffer(stream_arcfour_state *ss, byte *buf, int buf_size);

#endif /* sarc4_INCLUDED */
