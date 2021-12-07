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

/* $Id: gscdevn.h,v 1.9 2007/09/11 15:24:03 Arabidopsis Exp $ */
/* Client interface to DeviceN color */

#ifndef gscdevn_INCLUDED
#  define gscdevn_INCLUDED

#include "gscspace.h"

/*
 * Construct a new a DeviceN color space.
 *
 * Note that the client is responsible for memory management of the
 * tint transform Function.
 */
int
gs_cspace_new_DeviceN(
    gs_color_space **ppcs,
    uint num_components,
    gs_color_space *palt_cspace,
    gs_memory_t *pmem
    );


/* Set the tint transformation procedure for a DeviceN color space. */
/* VMS limits procedure names to 31 characters, and some systems only */
/* compare the first 23 characters. */
extern int gs_cspace_set_devn_proc(
				      gs_color_space * pcspace,
			int (*proc)(const float *,
				       float *,
				       const gs_imager_state *,
				       void *
				      ),
				      void *proc_data
				      );

/* Set the DeviceN tint transformation procedure to a Function. */
#ifndef gs_function_DEFINED
typedef struct gs_function_s gs_function_t;
#  define gs_function_DEFINED
#endif
int gs_cspace_set_devn_function(gs_color_space *pcspace,
				   gs_function_t *pfn);

/*
 * If the DeviceN tint transformation procedure is a Function,
 * return the function object, otherwise return 0.
 */
gs_function_t *gs_cspace_get_devn_function(const gs_color_space *pcspace);

/* Map a DeviceN color using a Function. */
int map_devn_using_function(const float *in, float *out,
			const gs_imager_state *pis, void *data);

/* Serialize a DeviceN map. */
int gx_serialize_device_n_map(const gs_color_space * pcs, gs_device_n_map * m, stream * s);

/*
 * DeviceN and NChannel color spaces can have an attributes dict.  In the
 * attribute dict can be a Colorants dict which contains Separation color
 * spaces.  If the Colorant dict is present, the PS logic will build each of
 * the Separation color spaces in a temp gstate and then call this procedure
 * to attach the Separation color space to the DeviceN color space.
 * The parameter to this procedure is a colorant name.  The Separation
 * color space is in the current (temp) gstate.  The DeviceN color space is
 * in the next gstate down in the gstate list (pgs->saved).
 */
int gs_attachattributecolorspace(gs_separation_name sep_name, gs_state * pgs);

#endif /* gscdevn_INCLUDED */
