/* Copyright (C) 2001-2007 Artifex Software, Inc.
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

"é)é*/
/* $Id: zncdummy.c,v 1.3 2008/03/23 15:27:43 Arabidopsis Exp $ */
/* Sample implementation for client custom processing of color spaces. */

/*
 * This module has been created to demonstrate how to add support for the use
 * of custom color handling to the Ghostscript graphics library via a custom color
 * callback mechanism.
 *
 * See the comments at the start of src/gsncdummy.c for more information.
 */

#include "math_.h"
#include "memory_.h"
#include "gx.h"
#include "gscdefs.h"
#include "gsnamecl.h"
#include "malloc_.h"
#include "ghost.h"
#include "oper.h"
#include "gsncdummy.h"

#if ENABLE_CUSTOM_COLOR_CALLBACK

extern pantone_list_t pantone_list[];
extern client_custom_color_procs_t demo_procs;

/*
 * Demo version of the PANTONE call back parameter structure.
 */
client_custom_color_params_t demo_callback = {
    &demo_procs,
    /*
     * Use our 'list' of Pantone colors as an example data.
     */
    (void *)(&pantone_list)
};

#endif 			/* ENABLE_CUSTOM_COLOR_CALLBACK */

/*
 * This procedure is here to simplify debugging.  Normally one would expect the
 * custom color callback structure to be set up by a calling application.
 * Since I do not have a calling application, I need a simple way to setup the
 * callback parameter.  The callback parameter is passed as a string value.
 * This routine puts the address of our demo callback structure into the
 * provided string.
 *
 * This routine allows the demo version of the PANTONE logic to be enabled
 * by adding the following to the command line:
 *  -c "<< /CustomColorCallback 32 string .pantonecallback >> setsystemparams" -f
 */

/* <string> .pantonecallback <string> */
static int
zpantonecallback(i_ctx_t *i_ctx_p)
{
#if ENABLE_CUSTOM_COLOR_CALLBACK
    os_ptr op = osp;
    int val, idx, buf_pos = 3;
    size_t iptr;
#define PTR_STRING_SIZE (2 * size_of(void *) + 3)

    /* Verify that the string size is big enough for our output */
    check_type(*op, t_string);
    check_write(*op);
    if (r_size(op) < PTR_STRING_SIZE)
	return_error(e_rangecheck);

    /* Convert our call back parameter structure pointer into a string */
    op->value.bytes[0] = '1';
    op->value.bytes[1] = '6';
    op->value.bytes[2] = '#';
    iptr = (size_t)(&demo_callback);
    for (idx = ((int)size_of(size_t)) * 8 - 4; idx >= 0; idx -= 4) {
	val = (int)(iptr >> idx) & 0xf;
	op->value.bytes[buf_pos++] = (byte)((val <= 9) ? '0' + val
						       : 'a' - 10 + val);
    }
    r_size(op) = PTR_STRING_SIZE;
#endif 			/* ENABLE_CUSTOM_COLOR_CALLBACK */
    return 0;
}

/* ------ Initialization procedure ------ */

const op_def pantone_op_defs[] =
{
    {"1.pantonecallback", zpantonecallback},
    op_def_end(0)
};
