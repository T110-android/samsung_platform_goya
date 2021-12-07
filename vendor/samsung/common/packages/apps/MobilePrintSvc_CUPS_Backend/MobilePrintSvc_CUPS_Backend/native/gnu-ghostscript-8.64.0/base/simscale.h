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

/* $Id: simscale.h,v 1.3 2007/09/11 15:24:41 Arabidopsis Exp $ */
/* Definitions for image mask interpolation filter */
/* Requires scommon.h; strimpl.h if any templates are referenced */

#ifndef simscale_INCLUDED
#  define simscale_INCLUDED

typedef struct stream_imscale_state_s stream_imscale_state;

struct stream_imscale_state_s {
    stream_image_scale_state_common;
    byte *window;

    int src_y;
    int src_offset;
    int src_size;

    byte *dst;
    int dst_y;
    int dst_offset;
    int dst_size;
};

extern const stream_template s_imscale_template;

#endif /* simscale_INCLUDED */
