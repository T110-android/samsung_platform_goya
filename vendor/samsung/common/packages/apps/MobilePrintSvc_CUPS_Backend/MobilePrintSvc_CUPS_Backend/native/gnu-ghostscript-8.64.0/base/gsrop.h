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

/* $Id: gsrop.h,v 1.7 2007/09/11 15:23:44 Arabidopsis Exp $ */
/* RasterOp / transparency procedure interface */

#ifndef gsrop_INCLUDED
#  define gsrop_INCLUDED

#include "gsropt.h"

/* Procedural interface */

int gs_setrasterop(gs_state *, gs_rop3_t);
gs_rop3_t gs_currentrasterop(const gs_state *);
int gs_setsourcetransparent(gs_state *, bool);
bool gs_currentsourcetransparent(const gs_state *);
int gs_settexturetransparent(gs_state *, bool);
bool gs_currenttexturetransparent(const gs_state *);

/* Save/restore the combined logical operation. */
gs_logical_operation_t gs_current_logical_op(const gs_state *);
int gs_set_logical_op(gs_state *, gs_logical_operation_t);

#endif /* gsrop_INCLUDED */
