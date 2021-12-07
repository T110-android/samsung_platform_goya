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

/* $Id: gsjconf.h,v 1.7 2007/09/11 15:23:53 Arabidopsis Exp $ */
/* jconfig.h file for Independent JPEG Group code */

#ifndef gsjconf_INCLUDED
#  define gsjconf_INCLUDED

/*
 * We should have the following here:

#include "stdpre.h"

 * But because of the directory structure used to build the IJG library, we
 * actually concatenate stdpre.h on the front of this file instead to
 * construct the jconfig.h file used for the compilation.
 */

#include "arch.h"

/* See IJG's jconfig.doc for the contents of this file. */

#ifdef __PROTOTYPES__
#  define HAVE_PROTOTYPES
#endif

#define HAVE_UNSIGNED_CHAR
#define HAVE_UNSIGNED_SHORT
#undef CHAR_IS_UNSIGNED

#ifdef __STDC__			/* is this right? */
#  define HAVE_STDDEF_H
#  define HAVE_STDLIB_H
#endif

#undef NEED_BSD_STRINGS		/* WRONG */
#undef NEED_SYS_TYPES_H		/* WRONG */
#undef NEED_FAR_POINTERS
#undef NEED_SHORT_EXTERNAL_NAMES

#undef INCOMPLETE_TYPES_BROKEN

/* The following is documented in jmemsys.h, not jconfig.doc. */
#if ARCH_LOG2_SIZEOF_INT <= 1
#  undef MAX_ALLOC_CHUNK
#  define MAX_ALLOC_CHUNK 0xfff0
#endif

#ifdef JPEG_INTERNALS

#if ARCH_ARITH_RSHIFT == 0
#  define RIGHT_SHIFT_IS_UNSIGNED
#else
#  undef RIGHT_SHIFT_IS_UNSIGNED
#endif

#endif /* JPEG_INTERNALS */

#endif /* gsjconf_INCLUDED */
