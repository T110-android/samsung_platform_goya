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

/* $Id: icolor.h,v 1.7 2007/09/11 15:24:13 Arabidopsis Exp $ */
/* Declarations for transfer function & similar cache remapping */

#ifndef icolor_INCLUDED
#  define icolor_INCLUDED

/*
 * Define the number of stack slots needed for zcolor_remap_one.
 * The client is responsible for doing check_e/ostack or the equivalent
 * before calling zcolor_remap_one.
 */
extern const int zcolor_remap_one_ostack;
extern const int zcolor_remap_one_estack;

/*
 * Schedule the sampling and reloading of a cache.  Note that if
 * zcolor_remap_one recognize the procedure as being of a special form, it
 * may not schedule anything, but it still returns o_push_estack.  (This is
 * a change as of release 5.95; formerly, it returned 0 in this case.)
 */
int zcolor_remap_one(i_ctx_t *, const ref *, gx_transfer_map *,
		     const gs_state *, op_proc_t);

/* Reload a cache with entries in [0..1] after sampling. */
int zcolor_remap_one_finish(i_ctx_t *);

/* Reload a cache with entries in [-1..1] after sampling. */
int zcolor_remap_one_signed_finish(i_ctx_t *);

/* Recompute the effective transfer functions and invalidate the current */
/* color after cache reloading. */
int zcolor_reset_transfer(i_ctx_t *);

/* Invalidate the current color after cache reloading. */
int zcolor_remap_color(i_ctx_t *);

#endif /* icolor_INCLUDED */
