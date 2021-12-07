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

/* $Id: gscpm.h,v 1.8 2007/09/11 15:24:00 Arabidopsis Exp $ */
/* Charpath mode and cache device status definitions */

#ifndef gscpm_INCLUDED
#  define gscpm_INCLUDED

typedef enum {
    cpm_show,			/* *show (default, must be 0) */
    cpm_charwidth,		/* stringwidth rmoveto (not standard PS) */
    cpm_false_charpath,		/* false charpath */
    cpm_true_charpath,		/* true charpath */
    cpm_false_charboxpath,	/* false charboxpath (not standard PS) */
    cpm_true_charboxpath	/* true charboxpath (ditto) */
} gs_char_path_mode;

typedef enum {
    CACHE_DEVICE_NONE = 0,	/* default, must be 0 */
    CACHE_DEVICE_NOT_CACHING,	/* setcachedevice done but not caching */
    CACHE_DEVICE_NONE_AND_CLIP, /* setcachedevice done, not caching and applied gsave and clip */
    CACHE_DEVICE_CACHING	/* setcachedevice done and caching */
} gs_in_cache_device_t;

#endif /* gscpm_INCLUDED */
