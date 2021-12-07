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

/* $Id: gxhttile.h,v 1.7 2007/09/11 15:24:22 Arabidopsis Exp $ */
/* Halftone tile definition */
/* Requires gxbitmap.h */

#ifndef gxhttile_INCLUDED
#  define gxhttile_INCLUDED

/*
 * A halftone tile is just an ordinary bitmap tile, plus a couple of other
 * items associated with managing its existence in a tile cache.
 * (See gzht.h for details.)  We define this in its own file so that
 * clients of gx_device_color can access it.
 */

#ifndef gx_ht_tile_DEFINED
#  define gx_ht_tile_DEFINED
typedef struct gx_ht_tile_s gx_ht_tile;

#endif

struct gx_ht_tile_s {
    gx_strip_bitmap tiles;	/* the currently rendered tile */
    int level;			/* the cached gray level, i.e. */
    /* the number of spots whitened, */
    /* or -1 if the cache is empty */
    uint index;			/* the index of the tile within */
    /* the cache (for GC) */
};

#endif /* gxhttile_INCLUDED */
