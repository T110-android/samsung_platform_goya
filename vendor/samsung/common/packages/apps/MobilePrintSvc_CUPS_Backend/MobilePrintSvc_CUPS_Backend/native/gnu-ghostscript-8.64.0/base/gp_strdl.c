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

/* $Id: gp_strdl.c,v 1.7 2007/09/11 15:24:25 Arabidopsis Exp $ */
/* Default, stream-based readline implementation */
#include "std.h"
#include "gstypes.h"
#include "gsmemory.h"
#include "gp.h"

int
gp_readline_init(void **preadline_data, gs_memory_t * mem)
{
    return 0;
}

int
gp_readline(stream *s_in, stream *s_out, void *readline_data,
	    gs_const_string *prompt, gs_string * buf,
	    gs_memory_t * bufmem, uint * pcount, bool *pin_eol,
	    bool (*is_stdin)(const stream *))
{
    return sreadline(s_in, s_out, readline_data, prompt, buf, bufmem, pcount,
		     pin_eol, is_stdin);
}

void
gp_readline_finit(void *readline_data)
{
}
