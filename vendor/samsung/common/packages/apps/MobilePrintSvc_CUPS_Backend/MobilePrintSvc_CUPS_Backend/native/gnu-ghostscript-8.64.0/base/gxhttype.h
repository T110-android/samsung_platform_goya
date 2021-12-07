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

/* $Id: gxhttype.h,v 1.7 2007/09/11 15:24:39 Arabidopsis Exp $ */
/* Client halftone type enumeration */

#ifndef gxhttype_INCLUDED
#  define gxhttype_INCLUDED

/* Halftone types */
typedef enum {
    ht_type_none,		/* is this needed? */
    ht_type_screen,		/* set by setscreen */
    ht_type_colorscreen,	/* set by setcolorscreen */
    ht_type_spot,		/* Type 1 halftone dictionary */
    ht_type_threshold,		/* Type 3 halftone dictionary */
    ht_type_threshold2,		/* Extended Type 3 halftone dictionary */
				/* (Type 3 with either 8- or 16-bit */
				/* samples, bytestring instead of string */
				/* thresholds, and 1 or 2 rectangles) */
    ht_type_multiple,		/* Type 5 halftone dictionary */
    ht_type_multiple_colorscreen,  /* Type 5 halftone dictionary */
				/* created from Type 2 or Type 4 */
				/* halftone dictionary  */
    ht_type_client_order	/* client-defined, creating a gx_ht_order */
} gs_halftone_type;

#endif /* gxhttype_INCLUDED */
