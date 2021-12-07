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

/* $Id: macsystypes.h,v 1.8 2007/09/11 15:24:10 Arabidopsis Exp $ */

#ifndef __sys_types_h__
#define __sys_types_h__

#include <MacTypes.h>
#include <unix.h>
#define CHECK_INTERRUPTS

/* use a 64 bit type for color vectors. (from MacTypes.h)
   this is important for devicen support, but can be safely
   undef'd to fallback to a 32 bit representation  */
#define GX_COLOR_INDEX_TYPE UInt64

#define main gs_main

#if (0)
#define fprintf myfprintf
#define fputs myfputs
#define getenv mygetenv
int myfprintf(FILE *file, const char *fmt, ...);
int myfputs(const char *string, FILE *file);
#endif

/* Metrowerks CodeWarrior should define this */
#ifndef __MACOS__
#define __MACOS__
#endif

#endif /* __sys_types_h__ */
