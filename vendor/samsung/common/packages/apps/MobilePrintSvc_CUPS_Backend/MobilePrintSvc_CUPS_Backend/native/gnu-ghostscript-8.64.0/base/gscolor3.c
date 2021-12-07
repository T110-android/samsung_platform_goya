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

/* $Id: gscolor3.c,v 1.9 2007/09/11 15:23:59 Arabidopsis Exp $ */
/* "Operators" for LanguageLevel 3 color facilities */
#include "gx.h"
#include "gserrors.h"
#include "gsmatrix.h"		/* for gscolor2.h */
#include "gscolor3.h"
#include "gsptype2.h"
#include "gxcolor2.h"		/* for gxpcolor.h */
#include "gxcspace.h"		/* for gs_cspace_init */
#include "gxdcolor.h"		/* for gxpcolor.h */
#include "gxpcolor.h"		/* for gs_color_space_type_Pattern */
#include "gzstate.h"
#include "gzpath.h"
#include "gxpaint.h"		/* (requires gx_path) */
#include "gxshade.h"
#include "gscie.h"

/* setsmoothness */
int
gs_setsmoothness(gs_state * pgs, floatp smoothness)
{
    pgs->smoothness =
	(smoothness < 0 ? 0 : smoothness > 1 ? 1 : smoothness);
    return 0;
}

/* currentsmoothness */
float
gs_currentsmoothness(const gs_state * pgs)
{
    return pgs->smoothness;
}

/* shfill */
int
gs_shfill(gs_state * pgs, const gs_shading_t * psh)
{
    /*
     * shfill is equivalent to filling the current clipping path (or, if
     * clipping, its bounding box) with the shading, disregarding the
     * Background if any.  In order to produce reasonable high-level output,
     * we must implement this by calling gs_fill_path. 
     */
    gs_pattern2_template_t pat;
    gs_matrix imat;
    gs_client_color cc;
    gs_color_space *pcs;
    gx_device_color devc;
    int code;

    /* Must install the shading color space
       to allow check_DeviceN_component_names initialize
       the color component map.
     */
    /* Don't bother with saving the old color space, color,
       and cie_joint_caches,
       because .shfill is always called within gsave-grestore -
       see gs/lib . */
    code = gs_setcolorspace(pgs, psh->params.ColorSpace);
    if (code < 0)
	return 0;
    if (psh->params.cie_joint_caches != NULL) {
	pgs->cie_joint_caches = psh->params.cie_joint_caches;
	rc_increment(pgs->cie_joint_caches);
    }
    gs_pattern2_init(&pat);
    pat.Shading = psh;
    gs_make_identity(&imat);
    code = gs_make_pattern(&cc, (gs_pattern_template_t *)&pat, &imat, pgs,
			   pgs->memory);
    if (code < 0)
	return code;
    code = gs_pattern2_set_shfill(&cc);
    if (code < 0)
	return code;
    pcs = gs_cspace_alloc(pgs->memory, &gs_color_space_type_Pattern);
    if (pcs == NULL)
	return_error(gs_error_VMerror);

    pcs->params.pattern.has_base_space = false;
    code = pcs->type->remap_color(&cc, pcs, &devc, (gs_imager_state *)pgs,
				  pgs->device, gs_color_select_texture);
    if (code >= 0) {
	gx_device *dev = pgs->device;
	bool need_path = !dev_proc(dev, pattern_manage)(dev, gs_no_id, NULL,
		pattern_manage__shfill_doesnt_need_path);

	if (need_path) {
    	    gx_path cpath;

	    gx_path_init_local(&cpath, pgs->memory);
	    code = gx_cpath_to_path(pgs->clip_path, &cpath);
	    if (code >= 0)
		code = gx_fill_path(&cpath, &devc, pgs, gx_rule_winding_number,
				    pgs->fill_adjust.x, pgs->fill_adjust.y);
	    gx_path_free(&cpath, "gs_shfill");
	} else 
	    code = gx_fill_path(NULL, &devc, pgs, gx_rule_winding_number,
				pgs->fill_adjust.x, pgs->fill_adjust.y);
    }
    rc_decrement(pcs, "gs_shfill");
    gs_pattern_reference(&cc, -1);
    return code;
}
