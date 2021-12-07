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

/* $Id: sisparam.h,v 1.9 2008/05/04 14:34:44 Arabidopsis Exp $ */
/* Generic image scaling stream definitions */
/* Requires strimpl.h */

#ifndef sisparam_INCLUDED
#  define sisparam_INCLUDED

/*
 * Image scaling streams all use a common set of parameters to define the
 * input and output data.  That is what we define here.
 */

/* Input values */
/*typedef byte PixelIn; */  /* per BitsPerComponentIn */
/*#define MaxValueIn 255 */  /* per MaxValueIn */

/* Output values */
/*typedef byte PixelOut; */  /* per BitsPerComponentOut */
/*#define MaxValueOut 255 */  /* per MaxValueOut */

/*
 * The 'support' S of a digital filter is the value such that the filter is
 * guaranteed to be zero for all arguments outside the range [-S..S].  We
 * limit the support so that we can put an upper bound on the time required
 * to compute an output value and on the amount of storage required for
 * X-filtered input data; this also allows us to use pre-scaled fixed-point
 * values for the weights if we wish.
 *
 * 8x8 pixels should be enough for any reasonable application....
 */
#define LOG2_MAX_ISCALE_SUPPORT 3
#define MAX_ISCALE_SUPPORT (1 << LOG2_MAX_ISCALE_SUPPORT)

/* Define image scaling stream parameters. */
typedef struct stream_image_scale_params_s {
    int Colors;			/* >= 1 */
    int BitsPerComponentIn;	/* bits per input value, 8 or 16 */
    uint MaxValueIn;		/* max value of input component, */
				/* 0 < MaxValueIn < 1 << BitsPerComponentIn */
    int WidthIn, HeightIn;	/* > 0 */
    int BitsPerComponentOut;	/* bits per output value, 8 or 16 */
    uint MaxValueOut;		/* max value of output component, */
				/* 0 < MaxValueOut < 1 << BitsPerComponentOut*/
    int WidthOut, HeightOut;	/* > 0 */
    bool ColorPolarityAdditive;	/* needed by SpecialDownScale filter */
    int src_y_offset;		/* Offset of the subimage in the source image. */
    int EntireWidthIn;		/* Height of entire input image. */
    int EntireHeightIn;		/* Height of entire input image. */
    int EntireWidthOut;		/* Height of entire output image. */
    int EntireHeightOut;	/* Height of entire output image. */
} stream_image_scale_params_t;

/* Define a generic image scaling stream state. */

#define stream_image_scale_state_common\
    stream_state_common;\
    stream_image_scale_params_t params

typedef struct stream_image_scale_state_s {
    stream_image_scale_state_common;
} stream_image_scale_state;

#endif /* sisparam_INCLUDED */
