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

/* $Id: gxchrout.c,v 1.7 2007/09/11 15:24:06 Arabidopsis Exp $ */
#include "math_.h"
#include "gx.h"
#include "gxchrout.h"
#include "gxfarith.h"
#include "gxistate.h"

/*
 * Determine the flatness for rendering a character in an outline font.
 * This may be less than the flatness in the imager state.
 * The second argument is the default scaling for the font: 0.001 for
 * Type 1 fonts, 1.0 for TrueType fonts.
 */
double
gs_char_flatness(const gs_imager_state *pis, floatp default_scale)
{
    /*
     * Set the flatness to a value that is likely to produce reasonably
     * good-looking curves, regardless of its current value in the
     * graphics state.  If the character is very small, set the flatness
     * to zero, which will produce very accurate curves.
     */
    double cxx = fabs(pis->ctm.xx), cyy = fabs(pis->ctm.yy);

    if (is_fzero(cxx) || (cyy < cxx && !is_fzero(cyy)))
	cxx = cyy;
    if (!is_xxyy(&pis->ctm)) {
	double cxy = fabs(pis->ctm.xy), cyx = fabs(pis->ctm.yx);

	if (is_fzero(cxx) || (cxy < cxx && !is_fzero(cxy)))
	    cxx = cxy;
	if (is_fzero(cxx) || (cyx < cxx && !is_fzero(cyx)))
	    cxx = cyx;
    }
    /* Correct for the default scaling. */
    cxx *= 0.001 / default_scale;
    /* Don't let the flatness be worse than the default. */
    if (cxx > pis->flatness)
	cxx = pis->flatness;
    /* If the character is tiny, force accurate curves. */
    if (cxx < 0.2)
	cxx = 0;
    return cxx;
}
