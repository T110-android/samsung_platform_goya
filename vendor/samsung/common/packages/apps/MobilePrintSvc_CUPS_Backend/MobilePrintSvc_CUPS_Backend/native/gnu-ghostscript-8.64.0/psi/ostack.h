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

/* $Id: ostack.h,v 1.7 2007/09/11 15:23:48 Arabidopsis Exp $ */
/* Definitions for Ghostscript operand stack */

#ifndef ostack_INCLUDED
#  define ostack_INCLUDED

#include "iostack.h"
#include "icstate.h"		/* for access to op_stack */

/* Define the operand stack pointers for operators. */
#define iop_stack (i_ctx_p->op_stack)
#define o_stack (iop_stack.stack)

#define osbot (o_stack.bot)
#define osp (o_stack.p)
#define ostop (o_stack.top)

/* Macro to ensure enough room on the operand stack */
#define check_ostack(n)\
  if ( ostop - osp < (n) )\
    { o_stack.requested = (n); return_error(e_stackoverflow); }

/* Operand stack manipulation. */

/* Note that push sets osp to (the new value of) op. */
#define push(n)\
  BEGIN\
    if ( (op += (n)) > ostop )\
      { o_stack.requested = (n); return_error(e_stackoverflow); }\
    else osp = op;\
  END

/*
 * Note that the pop macro only decrements osp, not op.  For this reason,
 *
 *      >>>     pop should only be used just before returning,  <<<
 *      >>>     or else op must be decremented explicitly.      <<<
 */
#define pop(n) (osp -= (n))

/*
 * Note that the interpreter does not check for operand stack underflow
 * before calling the operator procedure.  There are "guard" entries
 * with invalid types and attributes just below the bottom of the
 * operand stack: if the operator returns with a typecheck error,
 * the interpreter checks for underflow at that time.
 * Operators that don't typecheck their arguments must check for
 * operand stack underflow explicitly; operators that take a variable
 * number of arguments must also check for stack underflow in those cases
 * where they expect more than their minimum number of arguments.
 * (This is because the interpreter can only recognize that a typecheck
 * is really a stackunderflow when the stack has fewer than the
 * operator's declared minimum number of entries.)
 */
#define check_op(nargs)\
  if ( op < osbot + ((nargs) - 1) ) return_error(e_stackunderflow)
/*
 * Similarly, in order to simplify some overflow checks, we allocate
 * a few guard entries just above the top of the o-stack.
 */

/*
 * The operand stack is implemented as a linked list of blocks:
 * operators that can push or pop an unbounded number of values, or that
 * access the entire o-stack, must take this into account.  These are:
 *      (int)copy  index  roll  clear  count  cleartomark
 *      counttomark  aload  astore  packedarray
 *      .get/.putdeviceparams .gethardwareparams
 */

#endif /* ostack_INCLUDED */
