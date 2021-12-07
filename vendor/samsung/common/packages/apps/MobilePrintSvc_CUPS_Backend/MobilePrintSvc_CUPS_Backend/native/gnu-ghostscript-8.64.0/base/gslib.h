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

/* $Id: gslib.h,v 1.8 2007/09/11 15:23:46 Arabidopsis Exp $ */
/* Library initialization and finalization interface */
/* Requires stdio.h, gsmemory.h */

#ifndef gslib_INCLUDED
#  define gslib_INCLUDED

/*
 * Initialize the library.  gs_lib_init does all of the initialization,
 * using the C heap for initial allocation; if a client wants the library to
 * use a different default allocator during initialization, it should call
 * gs_lib_init0 and then gs_lib_init1.
 */
int gs_lib_init(FILE * debug_out);
gs_memory_t *gs_lib_init0(FILE * debug_out);
int gs_lib_init1(gs_memory_t *);

/* Clean up after execution. */
void gs_lib_finit(int exit_status, int code, gs_memory_t *);

#endif /* gslib_INCLUDED */
