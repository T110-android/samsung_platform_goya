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

/*$Id: rinkj-byte-stream.h,v 1.4 2007/09/11 15:25:09 Arabidopsis Exp $ */
/* Bytestream abstraction for Rinkj driver. */

typedef struct _RinkjByteStream RinkjByteStream;

struct _RinkjByteStream {
  int (*write) (RinkjByteStream *self, const char *buf, int size);
};

int
rinkj_byte_stream_write (RinkjByteStream *bs, const char *buf, int size);

int
rinkj_byte_stream_puts (RinkjByteStream *bs, const char *str);

int
rinkj_byte_stream_printf (RinkjByteStream *bs, const char *fmt, ...);

int
rinkj_byte_stream_close (RinkjByteStream *bs);


RinkjByteStream *
rinkj_byte_stream_file_new (FILE *f);
