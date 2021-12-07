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

/* $Id: sbtx.h,v 1.7 2007/09/11 15:24:40 Arabidopsis Exp $ */
/* Definitions for ByteTranslate filters */
/* Requires scommon.h; strimpl.h if any templates are referenced */

#ifndef sbtx_INCLUDED
#  define sbtx_INCLUDED

/* ByteTranslateEncode/Decode */
typedef struct stream_BT_state_s {
    stream_state_common;
    byte table[256];
} stream_BT_state;
typedef stream_BT_state stream_BTE_state;
typedef stream_BT_state stream_BTD_state;

#define private_st_BT_state()	/* in sfilter1.c */\
  gs_private_st_simple(st_BT_state, stream_BT_state,\
    "ByteTranslateEncode/Decode state")
extern const stream_template s_BTE_template;
extern const stream_template s_BTD_template;

#endif /* sbtx_INCLUDED */
