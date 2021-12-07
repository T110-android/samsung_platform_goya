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

/* $Id: gxi16bit.c,v 1.7 2008/03/23 15:28:11 Arabidopsis Exp $ */
/* 16-bit image procedures */
#include "gx.h"
#include "memory_.h"
#include "gpcheck.h"
#include "gserrors.h"
#include "gxfixed.h"
#include "gxfrac.h"
#include "gxarith.h"
#include "gxmatrix.h"
#include "gsccolor.h"
#include "gspaint.h"
#include "gxdevice.h"
#include "gxcmap.h"
#include "gxdcolor.h"
#include "gxistate.h"
#include "gxdevmem.h"
#include "gxcpath.h"
#include "gximage.h"

/* ---------------- Unpacking procedures ---------------- */

static const byte *
sample_unpack_16(byte * bptr, int *pdata_x, const byte * data,
		 int data_x, uint dsize, const sample_map *ignore_smap, int spread,
		 int ignore_num_components_per_plane)
{
    /* Assuming an identity map for all components. */
    register frac *bufp = (frac *) bptr;
    uint dskip = data_x << 1;
    const byte *psrc = data + dskip;
#define inc_bufp(bp, n) bp = (frac *)((byte *)(bp) + (n))
    uint sample;
    int left = dsize - dskip;

    while (left > 2) {
	sample = ((uint) psrc[0] << 8) + psrc[1];
	*bufp = (frac)((frac_1 * (sample + 1)) >> 16);
	inc_bufp(bufp, spread);
	psrc += 2;
	left -= 2;
    }
    *pdata_x = 0;
    return bptr;
}

const sample_unpack_proc_t sample_unpack_16_proc = sample_unpack_16;

/* ---------------- Rendering procedures ---------------- */

/* After unpacking, 16-bit amples have the same format as 12-bit ones */
/* and can be rendered by the procedures from gxi12bit.c              */
