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

/* $Id: gxcdevn.h,v 1.8 2007/09/11 15:24:19 Arabidopsis Exp $ */
/* Internal definitions for DeviceN color spaces */

#ifndef gxcdevn_INCLUDED
#  define gxcdevn_INCLUDED

#include "gsrefct.h"
#include "gxcindex.h"

/* Cache for DeviceN color.  Note that currently this is a 1-entry cache. */
#ifndef gs_device_n_map_DEFINED
#  define gs_device_n_map_DEFINED
typedef struct gs_device_n_map_s gs_device_n_map;
#endif
struct gs_device_n_map_s {
    rc_header rc;
    int (*tint_transform)(const float *in, float *out,
                          const gs_imager_state *pis, void *data);
    void *tint_transform_data;
    bool cache_valid;
    float tint[GS_CLIENT_COLOR_MAX_COMPONENTS];
    frac conc[GX_DEVICE_COLOR_MAX_COMPONENTS];
};
#define private_st_device_n_map() /* in gscdevn.c */\
  gs_private_st_ptrs1(st_device_n_map, gs_device_n_map, "gs_device_n_map",\
    device_n_map_enum_ptrs, device_n_map_reloc_ptrs, tint_transform_data)

/* Allocate and initialize a DeviceN map. */
int alloc_device_n_map(gs_device_n_map ** ppmap, gs_memory_t * mem,
                       client_name_t cname);

#ifndef gs_device_n_attributes_DEFINED
#  define gs_device_n_attributes_DEFINED
typedef struct gs_device_n_attributes_s gs_device_n_attributes;
#endif
struct gs_device_n_attributes_s {
    rc_header rc;
    gs_separation_name colorant_name;
    gs_color_space *cspace;
    struct gs_device_n_attributes_s * next;	/* Linked list */
};
#define private_st_device_n_attributes() /* in gscdevn.c */\
  gs_private_st_ptrs2(st_device_n_attributes, gs_device_n_attributes, "gs_device_n_attributes",\
    device_n_attributes_enum_ptrs, device_n_attributes_reloc_ptrs, cspace, next)

/* Check if we are using the alternate color space */
bool using_alt_color_space(const gs_state * pgs);

#endif /* gxcdevn_INCLUDED */
