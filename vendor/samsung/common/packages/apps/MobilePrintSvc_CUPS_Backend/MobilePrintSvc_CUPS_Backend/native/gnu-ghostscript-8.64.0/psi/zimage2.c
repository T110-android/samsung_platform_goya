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

/* $Id: zimage2.c,v 1.7 2007/09/11 15:24:36 Arabidopsis Exp $ */
/* image operator extensions for Level 2 PostScript */
#include "memory_.h"
#include "ghost.h"
#include "gsimage.h"
#include "gxiparam.h"
#include "icstate.h"
#include "iimage2.h"
#include "igstate.h"		/* for igs */


/*
 * Process an image that has no explicit source data.  This isn't used by
 * standard Level 2, but it's a very small procedure and is needed by
 * both zdps.c and zdpnext.c.
 */
int
process_non_source_image(i_ctx_t *i_ctx_p, const gs_image_common_t * pic,
			 client_name_t cname)
{
    gx_image_enum_common_t *pie;
    int code = gs_image_begin_typed(pic, igs, false /****** WRONG ******/ ,
				    &pie);

    /* We didn't pass any data, so there's nothing to clean up. */
    return code;
}
