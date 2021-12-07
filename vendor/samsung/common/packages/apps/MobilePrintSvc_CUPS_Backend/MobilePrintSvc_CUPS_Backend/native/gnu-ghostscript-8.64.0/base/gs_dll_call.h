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
/*$Id: gs_dll_call.h,v 1.4 2007/09/11 15:24:11 Arabidopsis Exp $ */

/* calling convention macros for windows style dlls.
 */ 
#ifndef GS_DLL_CALL_H
#define GS_DLL_CALL_H


#ifdef __WINDOWS__
# define _Windows
#endif

#ifdef _Windows
# ifndef GSDLLEXPORT
#  define GSDLLEXPORT __declspec(dllexport)
# endif
# ifndef GSDLLAPI
#  define GSDLLAPI __stdcall
# endif
# ifndef GSDLLCALL
#  define GSDLLCALL __stdcall
# endif
#endif  /* _Windows */

#if defined(OS2) && defined(__IBMC__)
# ifndef GSDLLAPI
#  define GSDLLAPI _System
# endif
# ifndef GSDLLCALL
#  define GSDLLCALL _System
# endif
#endif	/* OS2 && __IBMC */

#ifdef __MACOS__
# pragma export on
#endif

#ifndef GSDLLEXPORT
# define GSDLLEXPORT
#endif
#ifndef GSDLLAPI
# define GSDLLAPI
#endif
#ifndef GSDLLCALL
# define GSDLLCALL
#endif

#if defined(__IBMC__)
# define GSDLLAPIPTR * GSDLLAPI
# define GSDLLCALLPTR * GSDLLCALL
#else
# define GSDLLAPIPTR GSDLLAPI *
# define GSDLLCALLPTR GSDLLCALL * 
#endif

#endif /* GS_DLL_CALL_H */
