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

/* $Id: gsmemlok.h,v 1.7 2007/09/11 15:24:37 Arabidopsis Exp $ */
/* Interface to monitor-locked heap memory allocator */

/* Initial version 2/1/98 by John Desrosiers (soho@crl.com) */

#if !defined(gsmemlok_INCLUDED)
#  define gsmemlok_INCLUDED

#include "gsmemory.h"
#include "gxsync.h"

/*
 * This allocator encapsulates another allocator with a mutex.
 * Note that it does not keep track of memory that it acquires:
 * thus free_all with FREE_ALL_DATA is a no-op.
 */

typedef struct gs_memory_locked_s {
    gs_memory_common;		/* interface outside world sees */
    gs_memory_t *target;	/* allocator to front */
    gx_monitor_t *monitor;	/* monitor to serialize access to functions */
} gs_memory_locked_t;

/* ---------- Public constructors/destructors ---------- */

/* Initialize a locked memory manager. */
int gs_memory_locked_init(
			  gs_memory_locked_t * lmem,	/* allocator to init */
			  gs_memory_t * target	/* allocator to monitor lock */
			  );

/* Release a locked memory manager. */
/* Note that this has no effect on the target. */
void gs_memory_locked_release(gs_memory_locked_t *lmem);

/* Get the target of a locked memory manager. */
gs_memory_t * gs_memory_locked_target(const gs_memory_locked_t *lmem);

#endif /*!defined(gsmemlok_INCLUDED) */
