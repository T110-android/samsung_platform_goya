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

/* $Id: gsmemret.h,v 1.7 2007/09/11 15:24:10 Arabidopsis Exp $ */
/* Interface to retrying memory allocator */

#if !defined(gsmemret_INCLUDED)
#  define gsmemret_INCLUDED

#include "gsmemory.h"

/*
 * This allocator encapsulates another allocator with a closure that is
 * called to attempt to free up memory if an allocation fails.
 * Note that it does not keep track of memory that it acquires:
 * thus free_all with FREE_ALL_DATA is a no-op.
 */
typedef struct gs_memory_retrying_s gs_memory_retrying_t;

/*
 * Define the procedure type for the recovery closure.
 */
typedef enum {
    RECOVER_STATUS_NO_RETRY,
    RECOVER_STATUS_RETRY_OK
} gs_memory_recover_status_t;
typedef gs_memory_recover_status_t (*gs_memory_recover_proc_t)
     (gs_memory_retrying_t *rmem, void *proc_data);

struct gs_memory_retrying_s {
    gs_memory_common;		/* interface outside world sees */
    gs_memory_t *target;	/* allocator to front */
    gs_memory_recover_proc_t recover_proc;
    void *recover_proc_data;
};

/* ---------- Public constructors/destructors ---------- */

/* Initialize a retrying memory manager. */
int gs_memory_retrying_init(
			    gs_memory_retrying_t * rmem, /* allocator to init */
			    gs_memory_t * target	/* allocator to wrap */
			    );

/* Release a retrying memory manager. */
/* Note that this has no effect on the target. */
void gs_memory_retrying_release(gs_memory_retrying_t *rmem);

/* Set the recovery closure of a retrying memory manager. */
void gs_memory_retrying_set_recover(gs_memory_retrying_t *rmem,
				    gs_memory_recover_proc_t recover_proc,
				    void *recover_proc_data);

/* Get the target of a retrying memory manager. */
gs_memory_t * gs_memory_retrying_target(const gs_memory_retrying_t *rmem);

#endif /*!defined(gsmemret_INCLUDED) */
