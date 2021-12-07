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

/* $Id: zdosio.c,v 1.8 2008/03/23 15:27:42 Arabidopsis Exp $ */
/* MS-DOS direct I/O operators. */
/* These should NEVER be included in a released configuration! */
#include "dos_.h"
#include "ghost.h"
#include "oper.h"
#include "store.h"

/* <port> .inport <word> */
static int
zinport(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    check_type(*op, t_integer);
    make_int(op, inport((int)op->value.intval));
    return 0;
}

/* <port> .inportb <byte> */
static int
zinportb(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    check_type(*op, t_integer);
    make_int(op, inportb((int)op->value.intval));
    return 0;
}

/* <port> <word> .outport - */
static int
zoutport(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    check_type(*op, t_integer);
    check_type(op[-1], t_integer);
    outport((int)op[-1].value.intval, (int)op->value.intval);
    pop(1);
    return 0;
}

/* <port> <byte> .outportb - */
static int
zoutportb(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    check_type(*op, t_integer);
    check_int_leu(op[-1], 0xff);
    outportb((int)op[-1].value.intval, (byte) op->value.intval);
    pop(1);
    return 0;
}

/* <loc> .peek <byte> */
static int
zpeek(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    check_type(*op, t_integer);
    make_int(op, *(byte *) (op->value.intval));
    return 0;
}

/* <loc> <byte> .poke - */
static int
zpoke(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    check_type(*op, t_integer);
    check_int_leu(op[-1], 0xff);
    *(byte *) (op[-1].value.intval) = (byte) op->value.intval;
    pop(1);
    return 0;
}

/* ------ Operator initialization ------ */

const op_def zdosio_op_defs[] =
{
    {"1.inport", zinport},
    {"1.inportb", zinportb},
    {"2.outport", zoutport},
    {"2.outportb", zoutportb},
    {"1.peek", zpeek},
    {"2.poke", zpoke},
    op_def_end(0)
};
