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

/* $Id: gxalpha.h,v 1.7 2007/09/11 15:24:40 Arabidopsis Exp $ */
/* Internal machinery for alpha channel support */

#ifndef gxalpha_INCLUDED
#  define gxalpha_INCLUDED

/*
 * As discussed in the classic Porter & Duff paper on compositing,
 * supporting alpha channel properly involves premultiplying color values
 * that are associated with non-unity alpha values.  After considerable
 * thrashing around trying to read between the lines of the spotty NeXT
 * documentation, we've concluded that the correct approach is to
 * premultiply towards whatever the color value 0 represents in the device's
 * native color space: black for DeviceGray and DeviceRGB (displays and some
 * file formats), white for DeviceCMYK (color printers), with a special hack
 * for monochrome printers TBD.  This makes things very easy internally, at
 * the expense of some inconsistency at the boundaries.
 *
 * For the record, the only places apparently affected by this decision
 * are the following:
 *      - alphaimage, if it doesn't assume premultiplication (see below)
 *      - readimage
 *      - The cmap_rgb_alpha_ procedures in gxcmap.c
 *      - [color]image, if they are supposed to use currentalpha (see below)
 *      - The compositing code in gsalphac.c
 *
 * The NeXT documentation also is very unclear as to how readimage,
 * alphaimage, and [color]image are supposed to work.  Our current
 * interpretation is the following:
 *
 *      - readimage reads pixels exactly as the device stores them
 *      (converted into DeviceGray or DeviceRGB space if the device
 *      uses a palette).  Pixels with non-unity alpha come out
 *      premultiplied, however the device stores them.
 *
 *      - alphaimage assumes the pixels are premultiplied as appropriate
 *      for the relevant color space.  This makes alphaimage and
 *      readimage complementary, i.e., the output of readimage is
 *      suitable as the input of alphaimage.
 *
 *      - [color]image disregard currentalpha, and treat all input as
 * opaque (alpha = 1).  */
/*
 * Just in case we ever change our minds about the direction of
 * premultiplication, uncommenting the following preprocessor definition is
 * supposed to produce premultiplication towards white.
 */
/*#define PREMULTIPLY_TOWARDS_WHITE */

#endif /* gxalpha_INCLUDED */
