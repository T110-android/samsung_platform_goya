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

/* $Id: zmisc3.c,v 1.9 2008/03/23 15:28:06 Arabidopsis Exp $ */
/* Miscellaneous LanguageLevel 3 operators */
#include "ghost.h"
#include "gscspace.h"		/* for gscolor2.h */
#include "gsmatrix.h"		/* ditto */
#include "gsclipsr.h"
#include "gscolor2.h"
#include "oper.h"
#include "igstate.h"
#include "store.h"

/* - clipsave - */
static int
zclipsave(i_ctx_t *i_ctx_p)
{
    return gs_clipsave(igs);
}

/* - cliprestore - */
static int
zcliprestore(i_ctx_t *i_ctx_p)
{
    return gs_cliprestore(igs);
}

/* <proc1> <proc2> .eqproc <bool> */
/*
 * Test whether two procedures are equal to depth 10.
 * This is the equality test used by idiom recognition in 'bind'.
 */
/* Adobe specifies maximum depth of 10 but 12 is needed */
/* to reproduce the observed behavior. CET 31-01-05 */
#define MAX_DEPTH 12 
typedef struct ref2_s {
    ref proc1, proc2;
} ref2_t;
static int
zeqproc(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    ref2_t stack[MAX_DEPTH + 1];
    ref2_t *top = stack;

    make_array(&stack[0].proc1, 0, 1, op - 1);
    make_array(&stack[0].proc2, 0, 1, op);
    for (;;) {
	long i;

	if (r_size(&top->proc1) == 0) {
	    /* Finished these arrays, go up to next level. */
	    if (top == stack) {
		/* We're done matching: it succeeded. */
		make_true(op - 1);
		pop(1);
		return 0;
	    }
	    --top;
	    continue;
	}
	/* Look at the next elements of the arrays. */
	i = r_size(&top->proc1) - 1;
	array_get(imemory, &top->proc1, i, &top[1].proc1);
	array_get(imemory, &top->proc2, i, &top[1].proc2);
	r_dec_size(&top->proc1, 1);
	++top;
	/*
	 * Amazingly enough, the objects' executable attributes are not
	 * required to match.  This means { x load } will match { /x load },
	 * even though this is clearly wrong.
	 */
#if 0
	if (r_has_attr(&top->proc1, a_executable) !=
	    r_has_attr(&top->proc2, a_executable)
	    )
	    break;
#endif
	if (obj_eq(imemory, &top->proc1, &top->proc2)) {
	    /* Names don't match strings. */
	    if (r_type(&top->proc1) != r_type(&top->proc2) &&
		(r_type(&top->proc1) == t_name ||
		 r_type(&top->proc2) == t_name)
		)
		break;
	    --top;		/* no recursion */
	    continue;
	}
	if (r_is_array(&top->proc1) && r_is_array(&top->proc2) &&
	    r_size(&top->proc1) == r_size(&top->proc2) &&
	    top < stack + (MAX_DEPTH - 1)
	    ) {
	    /* Descend into the arrays. */
	    continue;
	}
	break;
    }
    /* An exit from the loop indicates that matching failed. */
    make_false(op - 1);
    pop(1);
    return 0;
}

/* ------ Initialization procedure ------ */

const op_def zmisc3_op_defs[] =
{
    op_def_begin_ll3(),
    {"0cliprestore", zcliprestore},
    {"0clipsave", zclipsave},
    {"2.eqproc", zeqproc},
    op_def_end(0)
};
