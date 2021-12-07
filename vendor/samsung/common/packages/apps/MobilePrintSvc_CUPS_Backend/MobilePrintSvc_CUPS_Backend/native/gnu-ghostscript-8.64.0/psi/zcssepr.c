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

/* $Id: zcssepr.c,v 1.10 2008/03/23 15:27:38 Arabidopsis Exp $ */
/* Separation color space support */
#include "memory_.h"
#include "ghost.h"
#include "oper.h"
#include "gsstruct.h"
#include "gscolor.h"
#include "gsmatrix.h"		/* for gxcolor2.h */
#include "gxcspace.h"
#include "gxfixed.h"		/* ditto */
#include "gxcolor2.h"
#include "estack.h"
#include "ialloc.h"
#include "icsmap.h"
#include "ifunc.h"
#include "igstate.h"
#include "iname.h"
#include "ivmspace.h"
#include "store.h"
#include "gscsepr.h"
#include "gscdevn.h"
#include "gxcdevn.h"
#include "zht2.h"

/* Imported from gscsepr.c */
extern const gs_color_space_type gs_color_space_type_Separation;
/* Imported from gscdevn.c */
extern const gs_color_space_type gs_color_space_type_DeviceN;

/*
 * Adobe first created the separation colorspace type and then later created
 * the DeviceN colorspace.  Logically the separation colorspace is the same
 * as a DeviceN colorspace with a single component, except for the /None and
 * /All parameter values.  We treat the separation colorspace as a DeviceN
 * colorspace except for the /All case.
 */

/* - currentoverprint <bool> */
static int
zcurrentoverprint(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    push(1);
    make_bool(op, gs_currentoverprint(igs));
    return 0;
}

/* <bool> setoverprint - */
static int
zsetoverprint(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    check_type(*op, t_boolean);
    gs_setoverprint(igs, op->value.boolval);
    pop(1);
    return 0;
}

/* - .currentoverprintmode <int> */
static int
zcurrentoverprintmode(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    push(1);
    make_int(op, gs_currentoverprintmode(igs));
    return 0;
}

/* <int> .setoverprintmode - */
static int
zsetoverprintmode(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    int param;
    int code = int_param(op, max_int, &param);

    if (code < 0 || (code = gs_setoverprintmode(igs, param)) < 0)
	return code;
    pop(1);
    return 0;
}

/* ------ Initialization procedure ------ */

const op_def zcssepr_l2_op_defs[] =
{
    op_def_begin_level2(),
    {"0currentoverprint", zcurrentoverprint},
    {"0.currentoverprintmode", zcurrentoverprintmode},
    {"1setoverprint", zsetoverprint},
    {"1.setoverprintmode", zsetoverprintmode},
    op_def_end(0)
};
