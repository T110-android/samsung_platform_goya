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

/* $Id: gsalphac.h,v 1.7 2007/09/11 15:23:52 Arabidopsis Exp $ */
/* Alpha-compositing interface */

#ifndef gsalphac_INCLUDED
#  define gsalphac_INCLUDED

#include "gscompt.h"

/*
 * Define the compositing operations.  These values must match the ones in
 * dpsNeXT.h.
 */
typedef enum {
    composite_Clear = 0,
    composite_Copy,
    composite_Sover,
    composite_Sin,
    composite_Sout,
    composite_Satop,
    composite_Dover,
    composite_Din,
    composite_Dout,
    composite_Datop,
    composite_Xor,
    composite_PlusD,
    composite_PlusL,
#define composite_last composite_PlusL
    composite_Highlight,	/* (only for compositerect) */
#define compositerect_last composite_Highlight
    composite_Dissolve		/* (not for PostScript composite operators) */
#define composite_op_last composite_Dissolve
} gs_composite_op_t;

/*
 * Define parameters for alpha-compositing.
 */
typedef struct gs_composite_alpha_params_s {
    gs_composite_op_t op;
    float delta;		/* only for Dissolve */
} gs_composite_alpha_params_t;

/* Create an alpha-compositing object. */
int gs_create_composite_alpha(gs_composite_t ** ppcte,
			      const gs_composite_alpha_params_t * params,
			      gs_memory_t * mem);

#endif /* gsalphac_INCLUDED */
