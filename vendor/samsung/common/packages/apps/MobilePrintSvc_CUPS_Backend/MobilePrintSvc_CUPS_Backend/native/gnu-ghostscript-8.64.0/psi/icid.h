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

/* $Id: icid.h,v 1.10 2009/04/19 13:54:31 Arabidopsis Exp $ */
/* Interface to zcid.c, zfcid0.c */

#ifndef icid_INCLUDED
#  define icid_INCLUDED

#ifndef gs_cid_system_info_DEFINED
#  define gs_cid_system_info_DEFINED
typedef struct gs_cid_system_info_s gs_cid_system_info_t;
#endif

/* Get the information from a CIDSystemInfo dictionary. */
int cid_system_info_param(gs_cid_system_info_t *, const ref *);

/* Convert a CID into TT char code or to TT glyph index, using SubstNWP. */
/* Returns 1 if a glyph presents, 0 if not, <0 if error. */
int cid_to_TT_charcode(const gs_memory_t *mem, 
		       const ref *Decoding, const ref *TT_cmap,  
		       const ref *SubstNWP, 
                       uint nCID, uint *c, ref *src_type, ref *dst_type);

/* Create a CIDMap from a True Type cmap, Decoding and SubstNWP. */
int cid_fill_CIDMap(const gs_memory_t *mem, const ref *Decoding, const ref *TT_cmap, const ref *SubstNWP, 
                    int GDBytes, ref *CIDMap);

/* <cid9font> <cid> .type9mapcid <charstring> <font_index> */
int ztype9mapcid(i_ctx_t *i_ctx_p);

#endif /* icid_INCLUDED */
