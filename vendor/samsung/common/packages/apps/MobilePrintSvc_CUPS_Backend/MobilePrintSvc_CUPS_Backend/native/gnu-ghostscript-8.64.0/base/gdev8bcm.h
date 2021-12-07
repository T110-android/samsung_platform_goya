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
/* $Id: gdev8bcm.h,v 1.7 2007/09/11 15:23:52 Arabidopsis Exp $ */
/* 8-bit color map support */
/* Requires gxdevice.h (for gx_color_value) */

#ifndef gdev8bcm_INCLUDED
#  define gdev8bcm_INCLUDED

/*
 * The MS-DOS, MS Windows, and X Windows drivers all use (at least on
 * some platforms) an 8-bit color map in which some fraction is reserved
 * for a pre-allocated cube and some or all of the remainder is
 * allocated dynamically.  Since looking up colors in this map can be
 * a major performance bottleneck, we provide an efficient implementation
 * that can be shared among drivers.
 *
 * As a performance compromise, we only look up the top 5 bits of the
 * RGB value in the color map.  This compromises color quality very little,
 * and allows substantial optimizations.
 */

#define gx_8bit_map_size 323
#define gx_8bit_map_spreader 123	/* approx. 323 - (1.618 * 323) */
typedef struct gx_8bit_map_entry_s {
    ushort rgb;			/* key = 0rrrrrgggggbbbbb */
#define gx_8bit_no_rgb ((ushort)0xffff)
#define gx_8bit_rgb_key(r, g, b)\
  (((r >> (gx_color_value_bits - 5)) << 10) +\
   ((g >> (gx_color_value_bits - 5)) << 5) +\
   (b >> (gx_color_value_bits - 5)))
    short index;		/* value */
} gx_8bit_map_entry;
typedef struct gx_8bit_color_map_s {
    int count;			/* # of occupied entries */
    int max_count;		/* max # of occupied entries */
    gx_8bit_map_entry map[gx_8bit_map_size + 1];
} gx_8bit_color_map;

/* Initialize an 8-bit color map. */
void gx_8bit_map_init(gx_8bit_color_map *, int);

/* Look up a color in an 8-bit color map. */
/* Return -1 if not found. */
int gx_8bit_map_rgb_color(const gx_8bit_color_map *, gx_color_value,
			  gx_color_value, gx_color_value);

/* Test whether an 8-bit color map has room for more entries. */
#define gx_8bit_map_is_full(pcm)\
  ((pcm)->count == (pcm)->max_count)

/* Add a color to an 8-bit color map. */
/* Return -1 if the map is full. */
int gx_8bit_add_rgb_color(gx_8bit_color_map *, gx_color_value,
			  gx_color_value, gx_color_value);

#endif /* gdev8bcm_INCLUDED */
