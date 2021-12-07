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

/* $Id: gxtmap.h,v 1.7 2007/09/11 15:24:35 Arabidopsis Exp $ */
/* Definition of transfer mapping function */
/* (also used for black generation and undercolor removal) */

#ifndef gxtmap_INCLUDED
#  define gxtmap_INCLUDED

/* Common definition for mapping procedures. */
/* These are used for transfer functions, black generation, */
/* and undercolor removal. */
/* gx_transfer_map should probably be renamed gx_mapping_cache.... */

/* Define an abstract type for a transfer map. */
typedef struct gx_transfer_map_s gx_transfer_map;

/*
 * Define the type of a mapping procedure.  There are two forms of this.
 * The original form passed only the transfer map itself as an argument:
 */
typedef float (*gs_mapping_proc) (floatp, const gx_transfer_map *);

/*
 * Later, we recognized that this procedure should really be a general
 * closure:
 */
typedef float (*gs_mapping_closure_proc_t) (floatp value,
					    const gx_transfer_map * pmap,
					    const void *proc_data);
typedef struct gs_mapping_closure_s {
    gs_mapping_closure_proc_t proc;
    const void *data;
} gs_mapping_closure_t;

#endif /* gxtmap_INCLUDED */
