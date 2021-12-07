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

/* $Id: gstrap.c,v 1.8 2008/03/23 15:27:58 Arabidopsis Exp $ */
/* Setting trapping parameters and zones */
#include "string_.h"
#include "gx.h"
#include "gserrors.h"
#include "gstrap.h"
#include "gsparamx.h"

/* Put a float parameter. */
static bool
check_unit(float *pval)
{
    return (*pval >= 0 && *pval <= 1);
}
static bool
check_positive(float *pval)
{
    return (*pval > 0);
}
static int
trap_put_float_param(gs_param_list * plist, gs_param_name param_name,
		     float *pval, bool(*check) (float *pval), int ecode)
{
    int code;

    switch (code = param_read_float(plist, param_name, pval)) {
	case 0:
	    if ((*check) (pval))
		return 0;
	    code = gs_error_rangecheck;
	default:
	    ecode = code;
	    param_signal_error(plist, param_name, ecode);
	    break;
	case 1:
	    break;
    }
    return ecode;
}

/* settrapparams */
int
gs_settrapparams(gs_trap_params_t * pparams, gs_param_list * plist)
{
    gs_trap_params_t params;
    int ecode = 0;
    static const char *const trap_placement_names[] = {
	gs_trap_placement_names, 0
    };

    params = *pparams;
    ecode = trap_put_float_param(plist, "BlackColorLimit",
				 &params.BlackColorLimit, check_unit, ecode);
    ecode = trap_put_float_param(plist, "BlackDensityLimit",
				 &params.BlackDensityLimit,
				 check_positive, ecode);
    ecode = trap_put_float_param(plist, "BlackWidth",
				 &params.BlackWidth, check_positive, ecode);
    ecode = param_put_bool(plist, "Enabled",
			   &params.Enabled, ecode);
    ecode = param_put_bool(plist, "ImageInternalTrapping",
			   &params.ImageInternalTrapping, ecode);
    ecode = param_put_bool(plist, "ImagemaskTrapping",
			   &params.ImagemaskTrapping, ecode);
    ecode = param_put_int(plist, "ImageResolution",
			  &params.ImageResolution, ecode);
    if (params.ImageResolution <= 0)
	param_signal_error(plist, "ImageResolution",
			   ecode = gs_error_rangecheck);
    ecode = param_put_bool(plist, "ImageToObjectTrapping",
			   &params.ImageToObjectTrapping, ecode);
    {
	int placement = params.ImageTrapPlacement;

	ecode = param_put_enum(plist, "ImageTrapPlacement",
			       &placement, trap_placement_names, ecode);
	params.ImageTrapPlacement = placement;
    }
    ecode = trap_put_float_param(plist, "SlidingTrapLimit",
				 &params.SlidingTrapLimit, check_unit, ecode);
    ecode = trap_put_float_param(plist, "StepLimit",
				 &params.StepLimit, check_unit, ecode);
    ecode = trap_put_float_param(plist, "TrapColorScaling",
				 &params.TrapColorScaling, check_unit, ecode);
    ecode = trap_put_float_param(plist, "TrapWidth",
				 &params.TrapWidth, check_positive, ecode);
    if (ecode < 0)
	return ecode;
    *pparams = params;
    return 0;
}
