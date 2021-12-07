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

/* $Id: memory_.h,v 1.7 2007/09/11 15:24:17 Arabidopsis Exp $ */
/* Generic substitute for Unix memory.h */

#ifndef memory__INCLUDED
#  define memory__INCLUDED

/* We must include std.h before any file that includes sys/types.h. */
#include "std.h"

/******
 ****** Note: the System V bcmp routine only returns zero or non-zero,
 ****** unlike memcmp which returns -1, 0, or 1.
 ******/

#ifdef __TURBOC__
/* Define inline functions */
#  ifdef __WIN32__
#    define memcmp_inline(b1,b2,len) memcmp(b1,b2,len)
#  else
#    define memcmp_inline(b1,b2,len) __memcmp__(b1,b2,len)
#  endif
#  include <mem.h>
#else
	/* Not Turbo C, no inline functions */
#  define memcmp_inline(b1,b2,len) memcmp(b1,b2,len)
	/*
	 * Apparently the newer VMS compilers include prototypes
	 * for the mem... routines in <string.h>.  Unfortunately,
	 * gcc lies on Sun systems: it defines __STDC__ even if
	 * the header files in /usr/include are broken.
	 * However, Solaris systems, which define __svr4__, do have
	 * correct header files.
	 */
	/*
	 * The exceptions vastly outnumber the BSD4_2 "rule":
	 * these tests should be the other way around....
	 */
#  if defined(VMS) || defined(_POSIX_SOURCE) || (defined(__STDC__) && (!defined(sun) || defined(__svr4__))) || defined(_HPUX_SOURCE) || defined(__WATCOMC__) || defined(THINK_C) || defined(bsdi) || defined(__FreeBSD) || (defined(_MSC_VER) && _MSC_VER >= 1000)
#    include <string.h>
#  else
#    if defined(BSD4_2) || defined(UTEK)
extern bcopy(), bcmp(), bzero();

#	 define memcpy(dest,src,len) bcopy(src,dest,len)
#	 define memcmp(b1,b2,len) bcmp(b1,b2,len)
	 /* Define our own versions of missing routines (in gsmisc.c). */
#	 define MEMORY__NEED_MEMMOVE
#        include <sys/types.h>	/* for size_t */
#	 define MEMORY__NEED_MEMSET
#	 if defined(UTEK)
#          define MEMORY__NEED_MEMCHR
#        endif			/* UTEK */
#    else
#      include <memory.h>
#      if defined(__SVR3) || defined(sun)	/* Not sure this is right.... */
#	 define MEMORY__NEED_MEMMOVE
#        include <sys/types.h>	/* for size_t */
#      endif			/* __SVR3 or sun */
#    endif			/* BSD4_2 or UTEK */
#  endif			/* VMS, POSIX, ... */
#endif /* !__TURBOC__ */

/*
 * If we are profiling, substitute our own versions of memset, memcpy,
 * and memmove, in case profiling libraries aren't available.
 */
#ifdef PROFILE
#  define MEMORY__NEED_MEMCPY
#  define MEMORY__NEED_MEMMOVE
#  define MEMORY__NEED_MEMSET
#endif

/*
 * Declare substitutes for library procedures we supply.  We undef them
 * first, just in case we are substituting for an existing library facility
 * that happens to be implemented as a macro.
 */
#ifdef MEMORY__NEED_MEMMOVE
#  undef memmove
#  define memmove(dest,src,len) gs_memmove(dest,src,len)
void *gs_memmove(void *, const void *, size_t);
#endif
#ifdef MEMORY__NEED_MEMCPY
#  undef memcpy
#  define memcpy(dest,src,len) gs_memcpy(dest,src,len)
void *gs_memcpy(void *, const void *, size_t);
#endif
#ifdef MEMORY__NEED_MEMSET
#  undef memset
#  define memset(dest,ch,len) gs_memset(dest,ch,len)
void *gs_memset(void *, int, size_t);
#endif
#ifdef MEMORY__NEED_MEMCHR
#  undef memchr
#  define memchr(ptr,ch,len) gs_memchr(ptr,ch,len)
void *gs_memchr(const void *, int, size_t);
#endif

#endif /* memory__INCLUDED */
