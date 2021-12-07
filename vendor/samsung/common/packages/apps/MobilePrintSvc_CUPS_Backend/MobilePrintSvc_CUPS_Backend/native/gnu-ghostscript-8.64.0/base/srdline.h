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

/* $Id: srdline.h,v 1.7 2007/09/11 15:24:10 Arabidopsis Exp $ */
/* Interface for readline */
/* Requires gsmemory.h, gstypes.h */

#ifndef srdline_INCLUDED
#  define srdline_INCLUDED

/*
 * Read a line from s_in, starting at index *pcount in buf.  Start by
 * printing prompt on s_out.  If the string is longer than size - 1 (we need
 * 1 extra byte at the end for a null or an EOL), use bufmem to reallocate
 * buf; if bufmem is NULL, just return 1.  In any case, store in *pcount the
 * first unused index in buf.  *pin_eol is normally false; it should be set
 * to true (and true should be recognized) to indicate that the last
 * character read was a ^M, which should cause a following ^J to be
 * discarded.  is_stdin(s) returns true iff s is stdin: this is needed for
 * an obscure condition in the default implementation.
 */
#ifndef stream_DEFINED
#  define stream_DEFINED
typedef struct stream_s stream;
#endif
#define sreadline_proc(proc)\
  int proc(stream *s_in, stream *s_out, void *readline_data,\
	   gs_const_string *prompt, gs_string *buf,\
	   gs_memory_t *bufmem, uint *pcount, bool *pin_eol,\
	   bool (*is_stdin)(const stream *))

/* Declare the default implementation. */
extern sreadline_proc(sreadline);

#endif /* srdline_INCLUDED */
