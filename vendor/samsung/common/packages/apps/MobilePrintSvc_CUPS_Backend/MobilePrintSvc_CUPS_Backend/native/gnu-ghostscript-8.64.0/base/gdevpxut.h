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

/* $Id: gdevpxut.h,v 1.8 2007/09/11 15:23:55 Arabidopsis Exp $ */
/* Utilities for PCL XL generation */
/* Requires gdevpxat.h, gdevpxen.h, gdevpxop.h */

#ifndef gdevpxut_INCLUDED
#  define gdevpxut_INCLUDED

/* ---------------- High-level constructs ---------------- */

/* Write the file header, including the resolution. */
int px_write_file_header(stream *s, const gx_device *dev);

/* Write the page header, including orientation. */
int px_write_page_header(stream *s, const gx_device *dev);

/* Write the media selection command if needed, updating the media size. */
int px_write_select_media(stream *s, const gx_device *dev,
			  pxeMediaSize_t *pms,
			  byte *media_source,
			  int page, bool Duplex, bool Tumble);

/*
 * Write the file trailer.  Note that this takes a FILE *, not a stream *,
 * since it may be called after the stream is closed.
 */
int px_write_file_trailer(FILE *file);

/* ---------------- Low-level data output ---------------- */

/* Write a sequence of bytes. */
#define PX_PUT_LIT(s, bytes) px_put_bytes(s, bytes, sizeof(bytes))
void px_put_bytes(stream * s, const byte * data, uint count);

/* Utilities for writing data values. */
/* H-P printers only support little-endian data, so that's what we emit. */

#define DA(a) pxt_attr_ubyte, (a)
void px_put_a(stream * s, px_attribute_t a);
void px_put_ac(stream *s, px_attribute_t a, px_tag_t op);

#define DUB(b) pxt_ubyte, (byte)(b)
void px_put_ub(stream * s, byte b);
void px_put_uba(stream *s, byte b, px_attribute_t a);

#define DS(i) (byte)(i), (byte)((i) >> 8)
void px_put_s(stream * s, uint i);

#define DUS(i) pxt_uint16, DS(i)
void px_put_us(stream * s, uint i);
void px_put_usa(stream *s, uint i, px_attribute_t a);
void px_put_u(stream * s, uint i);

#define DUSP(ix,iy) pxt_uint16_xy, DS(ix), DS(iy)
void px_put_usp(stream * s, uint ix, uint iy);
void px_put_usq_fixed(stream * s, fixed x0, fixed y0, fixed x1, fixed y1);

void px_put_ss(stream * s, int i);
void px_put_ssp(stream * s, int ix, int iy);

void px_put_l(stream * s, ulong l);

void px_put_r(stream * s, floatp r);  /* no tag */
void px_put_rl(stream * s, floatp r);  /* pxt_real32 tag */

void px_put_data_length(stream * s, uint num_bytes);

#endif /* gdevpxut_INCLUDED */
