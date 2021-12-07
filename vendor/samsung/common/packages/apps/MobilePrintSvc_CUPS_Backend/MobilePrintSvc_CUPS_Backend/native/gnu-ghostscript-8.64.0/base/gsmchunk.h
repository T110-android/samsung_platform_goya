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

/* $Id: gsmchunk.h,v 1.1 2009/04/19 13:54:33 Arabidopsis Exp $ */
/* chunk consolidating wrapper on a base memory allocator */

#ifndef gsmchunk_INCLUDED
#  define gsmchunk_INCLUDED

#define CHUNK_SIZE		65536

/* ---------- Public constructors/destructors ---------- */

/* Initialize a gs_memory_chunk_t */
	/* -ve error code or 0 */
int gs_memory_chunk_wrap(gs_memory_t **wrapped,	/* chunk allocator init */
		      gs_memory_t * target );	/* base allocator */

/* Release a chunk memory manager and all of the memory it held */
void gs_memory_chunk_release(gs_memory_t *cmem);

/* ---------- Accessors ------------- */

/* Retrieve this allocator's target */
gs_memory_t *gs_memory_chunk_target(const gs_memory_t *cmem);

#ifdef DEBUG
    void gs_memory_chunk_dump_memory(const gs_memory_t *mem);

    int chunk_allocator_unit_test(gs_memory_t *mem);
#endif /* DEBUG */

#endif /* gsmchunk_INCLUDED */
