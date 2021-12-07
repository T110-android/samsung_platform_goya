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

/* $Id: gscssub.c,v 1.7 2007/09/11 15:24:12 Arabidopsis Exp $ */
/* Color space substitution "operators" */
#include "gx.h"
#include "gserrors.h"
#include "gscssub.h"
#include "gxcspace.h"		/* for st_color_space */
#include "gxdevcli.h"
#include "gzstate.h"

/* .setsubstitutecolorspace */
/* Note that, to support PDF, ICCBased color spaces may be used to substitute
 * for the Device* color spaces (previously, only CIEBased color spaces could
 * be used for this purpose). */
int
gs_setsubstitutecolorspace(gs_state *pgs, gs_color_space_index csi,
			   const gs_color_space *pcs)
{
    int index = (int)csi;
    static const uint masks[3] = {
	(1 << gs_color_space_index_DeviceGray) |
	  (1 << gs_color_space_index_CIEA),
	(1 << gs_color_space_index_DeviceRGB) |
	  (1 << gs_color_space_index_CIEABC) |
	  (1 << gs_color_space_index_CIEDEF),
	(1 << gs_color_space_index_DeviceCMYK) |
	  (1 << gs_color_space_index_CIEDEFG)
    };
    const gs_color_space *pcs_old;

    if (index < 0 || index > 2)
	return_error(gs_error_rangecheck);
    if (pcs) {
        if (gs_color_space_get_index(pcs) == gs_color_space_index_CIEICC) {
            static const byte dev_ncomps[3] = {1, 3, 4};

             if (dev_ncomps[index] != cs_num_components(pcs))
                 return_error(gs_error_rangecheck);
        } else if (!masks[index] && (1 << gs_color_space_get_index(pcs)))
	    return_error(gs_error_rangecheck);
    }
    pcs_old = pgs->device_color_spaces.indexed[index];
    if (pcs_old == 0 &&	(pcs == 0 || gs_color_space_get_index(pcs) == csi))
	return 0;
    rc_assign(pgs->device_color_space.indexed[index],
	      (pcs ? pcs :
	       pgs->shared->device_color_spaces.indexed[index]),
	      "gs_setsubstitutecolorspace");
    return 0;
}

/* Possibly-substituted color space accessors. */
const gs_color_space *
gs_current_DeviceGray_space(const gs_state *pgs)
{
    const gs_color_space *pcs;

    return (!pgs->device->UseCIEColor ||
	    (pcs = pgs->device_color_spaces.named.Gray) == 0 ?
	    pgs->shared->device_color_spaces.named.Gray : pcs);
}
const gs_color_space *
gs_current_DeviceRGB_space(const gs_state *pgs)
{
    const gs_color_space *pcs;

    return (!pgs->device->UseCIEColor ||
	    (pcs = pgs->device_color_spaces.named.RGB) == 0 ?
	    pgs->shared->device_color_spaces.named.RGB : pcs);
}
const gs_color_space *
gs_current_DeviceCMYK_space(const gs_state *pgs)
{
    const gs_color_space *pcs;

    return (!pgs->device->UseCIEColor ||
	    (pcs = pgs->device_color_spaces.named.CMYK) == 0 ?
	    pgs->shared->device_color_spaces.named.CMYK : pcs);
}

/* .currentsubstitutecolorspace */
const gs_color_space *
gs_currentsubstitutecolorspace(const gs_state *pgs, gs_color_space_index csi)
{
    switch (csi) {
    case gs_color_space_index_DeviceGray:
	return gs_current_DeviceGray_space(pgs);
    case gs_color_space_index_DeviceRGB:
	return gs_current_DeviceRGB_space(pgs);
    case gs_color_space_index_DeviceCMYK:
	return gs_current_DeviceCMYK_space(pgs);
    default:
	return 0;
    }
}
