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

/* $Id: sbcp.h,v 1.7 2007/09/11 15:23:54 Arabidopsis Exp $ */
/* Interface to [T]BCP streams */

#ifndef sbcp_INCLUDED
#  define sbcp_INCLUDED

/* (T)BCPEncode */
/* (no state) */
extern const stream_template s_BCPE_template;
extern const stream_template s_TBCPE_template;

/* (T)BCPDecode */
typedef struct stream_BCPD_state_s {
    stream_state_common;
    /* The client sets the following before initialization. */
    int (*signal_interrupt) (stream_state *);
    int (*request_status) (stream_state *);
    /* The following are updated dynamically. */
    bool escaped;
    int matched;		/* TBCP only */
    int copy_count;		/* TBCP only */
    const byte *copy_ptr;	/* TBCP only */
} stream_BCPD_state;

#define private_st_BCPD_state()	/* in sbcp.c */\
  gs_private_st_simple(st_BCPD_state, stream_BCPD_state, "(T)BCPDecode state")
extern const stream_template s_BCPD_template;
extern const stream_template s_TBCPD_template;

#endif /* sbcp_INCLUDED */
