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

/* $Id: iimage2.h,v 1.7 2007/09/11 15:24:39 Arabidopsis Exp $ */
/* Level 2 image operator support */
/* Requires gsiparam.h */

#ifndef iimage2_INCLUDED
#  define iimage2_INCLUDED

/* This procedure is exported by zimage2.c for other modules. */

/*
 * Process an image that has no explicit source data.  This isn't used by
 * standard Level 2, but it's a very small procedure and is needed by
 * both zdps.c and zdpnext.c.
 */
int process_non_source_image(i_ctx_t *i_ctx_p,
                             const gs_image_common_t * pim,
                             client_name_t cname);

#endif /* iimage2_INCLUDED */
