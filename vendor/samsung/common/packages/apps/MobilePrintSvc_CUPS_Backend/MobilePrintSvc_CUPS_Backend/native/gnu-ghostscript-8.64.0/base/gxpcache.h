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

/* $Id: gxpcache.h,v 1.7 2007/09/11 15:24:01 Arabidopsis Exp $ */
/* Definition of Pattern cache */

#ifndef gxpcache_INCLUDED
#  define gxpcache_INCLUDED

/*
 * Define a cache for rendered Patterns.  This is currently an open
 * hash table with single probing (no reprobing) and round-robin
 * replacement.  Obviously, we can do better in both areas.
 */
#ifndef gx_pattern_cache_DEFINED
#  define gx_pattern_cache_DEFINED
typedef struct gx_pattern_cache_s gx_pattern_cache;

#endif
#ifndef gx_color_tile_DEFINED
#  define gx_color_tile_DEFINED
typedef struct gx_color_tile_s gx_color_tile;

#endif
struct gx_pattern_cache_s {
    gs_memory_t *memory;
    gx_color_tile *tiles;
    uint num_tiles;
    uint tiles_used;
    uint next;			/* round-robin index */
    ulong bits_used;
    ulong max_bits;
    void (*free_all) (gx_pattern_cache *);
};

#define private_st_pattern_cache() /* in gxpcmap.c */\
  gs_private_st_ptrs1(st_pattern_cache, gx_pattern_cache,\
    "gx_pattern_cache", pattern_cache_enum, pattern_cache_reloc, tiles)

#endif /* gxpcache_INCLUDED */
