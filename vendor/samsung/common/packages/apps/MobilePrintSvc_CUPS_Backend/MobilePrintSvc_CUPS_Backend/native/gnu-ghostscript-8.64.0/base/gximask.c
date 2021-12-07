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

/* $Id: gximask.c,v 1.6 2009/04/19 13:54:31 Arabidopsis Exp $ */
/* Functions for masked fill optimization. */
#include "gx.h"
#include "memory_.h"
#include "gserrors.h"
#include "gsptype1.h"
#include "gsptype2.h"
#include "gxdevice.h"
#include "gxdcolor.h"
#include "gxcpath.h"
#include "gximask.h"
#include "gzacpath.h"
#include "gzcpath.h"


/* Functions for masked fill optimization. */
/* Imagemask with a shading color would paint entire shading for each rectangle of the mask.
   These functions convert the mask into a clipping path and then render entire shading
   at once through it.
*/

int
gx_image_fill_masked_start(gx_device *dev, const gx_device_color *pdevc, const gx_clip_path *pcpath, 
			   gs_memory_t *mem, gx_device **cdev)
{
    if (gx_dc_is_pattern2_color(pdevc) || gx_dc_is_pattern1_color_clist_based(pdevc)) {
	if (!dev_proc(dev, pattern_manage)(dev, gs_no_id, NULL, pattern_manage__can_accum)) {
	    extern_st(st_device_cpath_accum);
	    gx_device_cpath_accum *pcdev =  gs_alloc_struct(mem, 
		    gx_device_cpath_accum, &st_device_cpath_accum, "gx_image_fill_masked_start");
	    gs_fixed_rect cbox;

	    if (pcdev == NULL)
		return_error(gs_error_VMerror);
	    gx_cpath_accum_begin(pcdev, mem);
	    gx_cpath_outer_box(pcpath, &cbox);
	    gx_cpath_accum_set_cbox(pcdev, &cbox);
	    pcdev->rc.memory = mem;
	    pcdev->width = dev->width;   /* For gx_default_copy_mono. */
	    pcdev->height = dev->height; /* For gx_default_copy_mono. */
	    gx_device_retain((gx_device *)pcdev, true);
	    *cdev = (gx_device *)pcdev;
	}
    } else
	*cdev = dev;
    return 0;
}

int 
gx_image_fill_masked_end(gx_device *dev, gx_device *tdev, const gx_device_color *pdevc)
{
    gx_device_cpath_accum *pcdev = (gx_device_cpath_accum *)dev;
    gx_clip_path cpath;
    gx_device_clip cdev;
    int code, code1;

    gx_cpath_init_local(&cpath, pcdev->memory);
    code = gx_cpath_accum_end(pcdev, &cpath);
    if (code >= 0)
	code = gx_dc_pattern2_clip_with_bbox_simple(pdevc, tdev, &cpath);
	gx_make_clip_device_on_stack(&cdev, &cpath, tdev);
    if (code >= 0 && pcdev->bbox.p.x < pcdev->bbox.q.x) {
	code1 = gx_device_color_fill_rectangle(pdevc, 
		    pcdev->bbox.p.x, pcdev->bbox.p.y, 
		    pcdev->bbox.q.x - pcdev->bbox.p.x, 
		    pcdev->bbox.q.y - pcdev->bbox.p.y, 
		    (gx_device *)&cdev, lop_default, 0);
	if (code == 0)
	    code = code1;
    }
    gx_device_retain((gx_device *)pcdev, false);
    gx_cpath_free(&cpath, "s_image_cleanup");
    return code;
}

int 
gx_image_fill_masked(gx_device *dev,
    const byte *data, int data_x, int raster, gx_bitmap_id id,
    int x, int y, int width, int height,
    const gx_device_color *pdc, int depth,
    gs_logical_operation_t lop, const gx_clip_path *pcpath)
{
    gx_device *cdev;
    int code;

    code = gx_image_fill_masked_start(dev, pdc, pcpath, dev->memory, &cdev);
    if (code >= 0)
	code = (*dev_proc(cdev, fill_mask))(cdev, data, data_x, raster, id,
			    x, y, width, height, pdc, depth, lop, pcpath);
    if (code >= 0 && cdev != dev)
	code = gx_image_fill_masked_end(cdev, dev, pdc);
    return code;
}
