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

/* $Id: gscpixel.h,v 1.8 2007/09/11 15:24:02 Arabidopsis Exp $ */
/* Interface to DevicePixel color space */
/* Requires gscspace.h */

#ifndef gscpixel_INCLUDED
#  define gscpixel_INCLUDED

/* Construct a new DevicePixel color space. */
int gs_cspace_new_DevicePixel(gs_memory_t *mem, gs_color_space **ppcs,
			      int depth);

#endif /* gscpixel_INCLUDED */
