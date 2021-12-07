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

/* $Id: zchar2.c,v 1.8 2008/03/23 15:28:15 Arabidopsis Exp $ */
/* Type 2 character display operator */
#include "ghost.h"
#include "oper.h"
#include "gxfixed.h"
#include "gxmatrix.h"
#include "gxfont.h"
#include "gxfont1.h"
#include "gxtype1.h"
#include "ichar1.h"

/* <font> <code|name> <name> <charstring> .type2execchar - */
static int
ztype2execchar(i_ctx_t *i_ctx_p)
{
    return charstring_execchar(i_ctx_p, (1 << (int)ft_encrypted2));
}

/* ------ Initialization procedure ------ */

const op_def zchar2_op_defs[] =
{
    {"4.type2execchar", ztype2execchar},
    op_def_end(0)
};
