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

/* $Id: icontext.h,v 1.7 2007/09/11 15:24:21 Arabidopsis Exp $ */
/* Externally visible context state */
/* Requires iref.h, stdio_.h */

#ifndef icontext_INCLUDED
#  define icontext_INCLUDED

#include "gsstype.h"		/* for extern_st */
#include "icstate.h"

/* Declare the GC descriptor for context states. */
extern_st(st_context_state);

/*
 * Define the procedure for resetting user parameters when switching
 * contexts. This is defined in either zusparam.c or inouparm.c.
 */
extern int set_user_params(i_ctx_t *i_ctx_p, const ref * paramdict);

/* Allocate the state of a context, always in local VM. */
/* If *ppcst == 0, allocate the state object as well. */
int context_state_alloc(gs_context_state_t ** ppcst,
			const ref *psystem_dict,
			const gs_dual_memory_t * dmem);

/* Load the state of the interpreter from a context. */
/* The argument is not const because caches may be updated. */
int context_state_load(gs_context_state_t *);

/* Store the state of the interpreter into a context. */
int context_state_store(gs_context_state_t *);

/* Free the contents of the state of a context, always to its local VM. */
/* Return a mask of which of its VMs, if any, we freed. */
int context_state_free(gs_context_state_t *);

#endif /* icontext_INCLUDED */
