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

/*$Id: gsequivc.h,v 1.4 2007/09/11 15:23:54 Arabidopsis Exp $ */
/* Header for routines for determining equivalent color for spot colors */

/* For more information, see comment at the start of src/gsequivc.c */

#ifndef gsequivc_INCLUDED
# define gsequivc_INCLUDED

/*
 * Structure for holding a CMYK color.
 */
typedef struct cmyk_color_s {
    bool color_info_valid;
    frac c;
    frac m;
    frac y;
    frac k;
} cmyk_color;

/*
 * Structure for holding parameters for collecting the equivalent CMYK
 * for a spot colorant.
 */
typedef struct equivalent_cmyk_color_params_s {
    bool all_color_info_valid;
    cmyk_color color[GX_DEVICE_MAX_SEPARATIONS];
} equivalent_cmyk_color_params;

/*
 * If possible, update the equivalent CMYK color for spot colors.
 */
void update_spot_equivalent_cmyk_colors(gx_device * pdev,
		const gs_state * pgs, gs_devn_params * pdevn_params,
		equivalent_cmyk_color_params * pparams);

/*
 * Utiliy routine:  Capture equivalent color when given a modified
 * color space.
 */
void capture_spot_equivalent_cmyk_colors(gx_device * pdev,
		const gs_state * pgs, const gs_client_color * pcc,
		const gs_color_space * pcs, int sep_num,
		equivalent_cmyk_color_params * pparams);

#endif		/* define gsequivc_INCLUDED */
