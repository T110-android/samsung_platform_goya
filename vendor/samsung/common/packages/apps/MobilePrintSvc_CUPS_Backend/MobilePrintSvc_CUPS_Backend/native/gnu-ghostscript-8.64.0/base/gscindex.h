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

/* $Id: gscindex.h,v 1.8 2009/04/19 13:54:31 Arabidopsis Exp $ */
/* Client interface to Indexed color facilities */

#ifndef gscindex_INCLUDED
#  define gscindex_INCLUDED

#include "gscspace.h"
#include "gxfrac.h"

/*
 * Indexed color spaces.
 *
 * If the color space will use a procedure rather than a byte table,
 * ptbl should be set to 0.
 *
 * Unlike most of the other color space constructors, this one initializes
 * some of the fields of the colorspace. In the case in which a string table
 * is used for mapping, it initializes the entire structure. Note that the
 * client is responsible for the table memory in that case; the color space
 * will not free it when the color space itself is released.
 *
 * For the case of an indexed color space based on a procedure, a default
 * procedure will be provided that simply echoes the color values already in
 * the palette; the client may override these procedures by use of
 * gs_cspace_indexed_set_proc. If the client wishes to insert values into
 * the palette, it should do so by using gs_cspace_indexed_value_array, and
 * directly inserting the desired values into the array.
 *
 * If the client does insert values into the palette directly, the default
 * procedures provided by the client are fairly efficient, and there are
 * few instances in which the client would need to replace them.
 */
extern int gs_cspace_build_Indexed(
				   gs_color_space ** ppcspace,
				   gs_color_space * pbase_cspace,
				   uint num_entries,
				   const gs_const_string * ptbl,
				   gs_memory_t * pmem
				   );

/* Return the number of entries in the palette of an indexed color space. */
extern int gs_cspace_indexed_num_entries(
					 const gs_color_space * pcspace
					 );

/* In the case of a procedure-based indexed color space, get a pointer to */
/* the array of cached values. */
extern float *gs_cspace_indexed_value_array(
					    const gs_color_space * pcspace
					    );

/* Set the lookup procedure to be used for an Indexed color space. */
extern int gs_cspace_indexed_set_proc(
				      gs_color_space * pcspace,
				      int (*proc) (const gs_color_space *, int, float *)
				      );

/* Look up an index in an Indexed color space. */
int gs_cspace_indexed_lookup(const gs_color_space *, int,
			     gs_client_color *);

/* Look up an index in an Indexed color space. Return as a byte */
int gs_cspace_indexed_lookup_bytes(const gs_color_space *pcs, float index_float,
			unsigned char *output);

/* Look up an index in an Indexed color space. Return as a frac */
int gs_cspace_indexed_lookup_frac(const gs_color_space *pcs, float index_float,
			frac *output);

#endif /* gscindex_INCLUDED */
