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

/* $Id: gxdither.h,v 1.8 2007/09/11 15:23:59 Arabidopsis Exp $ */
/* Interface to gxdither.c */

#ifndef gxdither_INCLUDED
#  define gxdither_INCLUDED

#include "gxfrac.h"

#ifndef gx_device_halftone_DEFINED
#  define gx_device_halftone_DEFINED
typedef struct gx_device_halftone_s gx_device_halftone;
#endif

/*
 * Render DeviceN possibly by halftoning.
 *  pcolors = pointer to an array color values (as fracs)
 *  pdevc - pointer to device color structure
 *  dev = pointer to device data structure
 *  pht = pointer to halftone data structure
 *  ht_phase  = halftone phase
 *  This is part of a kludge to minimize differences in the
 *  regression testing.
 */
int gx_render_device_DeviceN(frac * pcolor, gx_device_color * pdevc,
    gx_device * dev, gx_device_halftone * pdht, const gs_int_point * ht_phase);
/*
 * Reduce a colored halftone with 0 or 1 varying plane(s) to a pure color
 * or a binary halftone.
 */
int gx_devn_reduce_colored_halftone(gx_device_color *pdevc, gx_device *dev);

#endif /* gxdither_INCLUDED */
