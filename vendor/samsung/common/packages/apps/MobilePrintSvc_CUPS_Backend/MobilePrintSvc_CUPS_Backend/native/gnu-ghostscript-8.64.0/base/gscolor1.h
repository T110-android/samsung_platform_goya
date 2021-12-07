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

/* $Id: gscolor1.h,v 1.7 2007/09/11 15:24:31 Arabidopsis Exp $ */
/* Client interface to Level 1 extended color facilities */
/* Requires gscolor.h */

#ifndef gscolor1_INCLUDED
#  define gscolor1_INCLUDED

/* Color and gray interface */
int gs_setcmykcolor(gs_state *, floatp, floatp, floatp, floatp),
    gs_currentcmykcolor(const gs_state *, float[4]),
    gs_setblackgeneration(gs_state *, gs_mapping_proc),
    gs_setblackgeneration_remap(gs_state *, gs_mapping_proc, bool);
gs_mapping_proc gs_currentblackgeneration(const gs_state *);
int gs_setundercolorremoval(gs_state *, gs_mapping_proc),
    gs_setundercolorremoval_remap(gs_state *, gs_mapping_proc, bool);
gs_mapping_proc gs_currentundercolorremoval(const gs_state *);

/* Transfer function */
int gs_setcolortransfer(gs_state *, gs_mapping_proc /*red */ ,
			gs_mapping_proc /*green */ ,
			gs_mapping_proc /*blue */ ,
			gs_mapping_proc /*gray */ ),
    gs_setcolortransfer_remap(gs_state *, gs_mapping_proc /*red */ ,
			      gs_mapping_proc /*green */ ,
			      gs_mapping_proc /*blue */ ,
			      gs_mapping_proc /*gray */ , bool);
void gs_currentcolortransfer(const gs_state *, gs_mapping_proc[4]);

#endif /* gscolor1_INCLUDED */
