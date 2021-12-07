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

/* $Id: gsht.h,v 1.7 2007/09/11 15:24:10 Arabidopsis Exp $ */
/* Public interface to halftone functionality */

#ifndef gsht_INCLUDED
#  define gsht_INCLUDED

/* Client definition of (Type 1) halftones */
typedef struct gs_screen_halftone_s {
    float frequency;
    float angle;
    float (*spot_function) (floatp, floatp);
    /* setscreen or sethalftone sets these: */
    /* (a Level 2 feature, but we include them in Level 1) */
    float actual_frequency;
    float actual_angle;
} gs_screen_halftone;

#define st_screen_halftone_max_ptrs 0

/* Client definition of color (Type 2) halftones */
typedef struct gs_colorscreen_halftone_s {
    union _css {
	gs_screen_halftone indexed[4];
	struct _csc {
	    gs_screen_halftone red, green, blue, gray;
	} colored;
    } screens;
} gs_colorscreen_halftone;

#define st_colorscreen_halftone_max_ptrs 0

/* Procedural interface */
int gs_setscreen(gs_state *, gs_screen_halftone *);
int gs_currentscreen(const gs_state *, gs_screen_halftone *);
int gs_currentscreenlevels(const gs_state *);

/*
 * Enumeration-style definition of a single screen.  The client must:
 *      - probably, call gs_screen_enum_alloc;
 *      - call gs_screen_init;
 *      - in a loop,
 *              - call gs_screen_currentpoint; if it returns 1, exit;
 *              - call gs_screen_next;
 *      - if desired, call gs_screen_install to install the screen.
 */
typedef struct gs_screen_enum_s gs_screen_enum;
gs_screen_enum *gs_screen_enum_alloc(gs_memory_t *, client_name_t);
int gs_screen_init(gs_screen_enum *, gs_state *, gs_screen_halftone *);
int gs_screen_currentpoint(gs_screen_enum *, gs_point *);
int gs_screen_next(gs_screen_enum *, floatp);
int gs_screen_install(gs_screen_enum *);

#endif /* gsht_INCLUDED */
