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

/* $Id: gsdps.c,v 1.8 2008/03/23 15:27:59 Arabidopsis Exp $ */
/* Display PostScript extensions */
#include "gx.h"
#include "gserrors.h"
#include "gsdps.h"
#include "gspath.h"		/* for newpath */
#include "gxdevice.h"		/* for gxcpath.h */
#include "gzpath.h"		/* for gzcpath.h */
#include "gzstate.h"
#include "gzcpath.h"

/* ---------------- View clipping ---------------- */

/* Forward references */
static int common_viewclip(gs_state *, int);

int
gs_initviewclip(gs_state * pgs)
{
    gx_clip_path *pcpath = pgs->view_clip;

    if (pcpath != 0 && pcpath->rule != 0) {
	gx_cpath_reset(pcpath);
	pcpath->rule = 0;
    }
    return 0;
}

int
gs_viewclip(gs_state * pgs)
{
    return common_viewclip(pgs, gx_rule_winding_number);
}

int
gs_eoviewclip(gs_state * pgs)
{
    return common_viewclip(pgs, gx_rule_even_odd);
}

/* This code is (almost) copied from common_clip in gspath.c. */
/* Someday we'll find a way to merge them. */
static int
common_viewclip(gs_state * pgs, int rule)
{
    gs_fixed_rect bbox;
    gx_clip_path rpath;
    int code;
    gx_clip_path *pcpath = pgs->view_clip;

    if (pcpath == 0) {
	pcpath = gx_cpath_alloc(pgs->memory, "gs_[eo]viewclip");
	if (pcpath == 0)
	    return_error(gs_error_VMerror);
	pgs->view_clip = pcpath;
    }
    if ((code = gx_path_bbox(pgs->path, &bbox)) < 0)
	return code;
    gx_cpath_init_local(&rpath, pgs->memory);
    code = gx_cpath_from_rectangle(&rpath, &bbox);
    if (code >= 0)
	code = gx_cpath_clip(pgs, &rpath, pgs->path, rule);
    if (code < 0) {
	gx_cpath_free(&rpath, "gs_[eo]viewclip");
	return code;
    }
    rpath.rule = rule;
    gx_cpath_assign_free(pcpath, &rpath);
    gs_newpath(pgs);
    return 0;
}

int
gs_viewclippath(gs_state * pgs)
{
    gx_path cpath;
    gx_clip_path *pcpath = pgs->view_clip;
    int code;

    gx_path_init_local(&cpath, pgs->memory);
    if (pcpath == 0 || pcpath->rule == 0) {
	/* No view clip path is active: fabricate one. */
	gs_fixed_rect box;

	code = gx_default_clip_box(pgs, &box);
	if (code < 0)
	    return code;
	code = gx_path_add_rectangle(&cpath, box.p.x, box.p.y,
				     box.q.x, box.q.y);
    } else {
	code = gx_cpath_to_path(pcpath, &cpath);
    }
    if (code < 0)
	return code;
    return gx_path_assign_free(pgs->path, &cpath);
}
