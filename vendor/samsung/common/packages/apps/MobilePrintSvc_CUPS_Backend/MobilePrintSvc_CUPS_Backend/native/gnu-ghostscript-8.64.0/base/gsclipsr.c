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

/* $Id: gsclipsr.c,v 1.8 2008/03/23 15:27:45 Arabidopsis Exp $ */
/* clipsave/cliprestore */
#include "gx.h"
#include "gserrors.h"
#include "gsclipsr.h"
#include "gsstruct.h"
#include "gxclipsr.h"
#include "gxfixed.h"		/* for gxpath.h */
#include "gxpath.h"
#include "gzstate.h"

/* Structure descriptors */
private_st_clip_stack();

/*
 * When we free a clip stack entry and the associated clip path.
 */
static void
rc_free_clip_stack(gs_memory_t * mem, void *vstack, client_name_t cname)
{
    gx_clip_stack_t *stack = (gx_clip_stack_t *)vstack;

    if (stack->rc.ref_count <= 1 ) {
        gx_clip_path *pcpath = stack->clip_path;

	gs_free_object(stack->rc.memory, stack, cname);
	gx_cpath_free(pcpath, "rc_free_clip_stack");
    }
}

/* clipsave */
int
gs_clipsave(gs_state *pgs)
{
    gs_memory_t *mem = pgs->memory;
    gx_clip_path *copy =
	gx_cpath_alloc_shared(pgs->clip_path, mem, "gs_clipsave(clip_path)");
    gx_clip_stack_t *stack =
	gs_alloc_struct(mem, gx_clip_stack_t, &st_clip_stack,
			"gs_clipsave(stack)");

    if (copy == 0 || stack == 0) {
	gs_free_object(mem, stack, "gs_clipsave(stack)");
	gs_free_object(mem, copy, "gs_clipsave(clip_path)");
	return_error(gs_error_VMerror);
    }
    rc_init_free(stack, mem, 1, rc_free_clip_stack);
    stack->clip_path = copy;
    stack->next = pgs->clip_stack;
    pgs->clip_stack = stack;
    return 0;
}

/* cliprestore */
int
gs_cliprestore(gs_state *pgs)
{
    gx_clip_stack_t *stack = pgs->clip_stack;

    if (stack) {
	gx_clip_stack_t *next = stack->next;
	gx_clip_path *pcpath = stack->clip_path;
	int code;

	if (stack->rc.ref_count == 1) {
	    /* Use assign_free rather than assign_preserve. */
	    gs_free_object(stack->rc.memory, stack, "cliprestore");
	    code = gx_cpath_assign_free(pgs->clip_path, pcpath);
	} else {
	    code = gx_cpath_assign_preserve(pgs->clip_path, pcpath);
	    if (code < 0)
		return code;
	    --(stack->rc.ref_count);
	}
	pgs->clip_stack = next;
	return code;
    } else {
	return gx_cpath_assign_preserve(pgs->clip_path, pgs->saved->clip_path);
    }
}
