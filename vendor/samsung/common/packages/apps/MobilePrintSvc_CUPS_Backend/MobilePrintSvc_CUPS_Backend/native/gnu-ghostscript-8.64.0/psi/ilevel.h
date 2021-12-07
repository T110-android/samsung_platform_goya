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

/* $Id: ilevel.h,v 1.7 2007/09/11 15:24:03 Arabidopsis Exp $ */
/* Interpreter language level interface */

#ifndef ilevel_INCLUDED
#  define ilevel_INCLUDED

/* The current interpreter language level */
#define LANGUAGE_LEVEL (i_ctx_p->language_level)
#define LL2_ENABLED (LANGUAGE_LEVEL >= 2)
#define LL3_ENABLED (LANGUAGE_LEVEL >= 3)
#define level2_enabled LL2_ENABLED	/* backward compatibility */

#endif /* ilevel_INCLUDED */
