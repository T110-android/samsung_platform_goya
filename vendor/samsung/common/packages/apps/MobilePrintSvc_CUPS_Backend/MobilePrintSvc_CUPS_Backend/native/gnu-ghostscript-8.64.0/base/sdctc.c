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

/* $Id: sdctc.c,v 1.9 2007/09/11 15:24:19 Arabidopsis Exp $ */
/* Code common to DCT encoding and decoding streams */
#include "stdio_.h"
#include "jpeglib_.h"
#include "strimpl.h"
#include "sdct.h"

public_st_DCT_state();

/* Set the defaults for the DCT filters. */
void
s_DCT_set_defaults(stream_state * st)
{
    stream_DCT_state *const ss = (stream_DCT_state *) st;

    ss->jpeg_memory = (gs_memory_t *)gs_lib_ctx_get_non_gc_memory_t(); /* cast away const */
    ss->data.common = 0;
	/****************
	  ss->data.common->Picky = 0;
	  ss->data.common->Relax = 0;
	 ****************/
    ss->ColorTransform = -1;
    ss->QFactor = 1.0;
    /* Clear pointers */
    ss->Markers.data = 0;
    ss->Markers.size = 0;
}
