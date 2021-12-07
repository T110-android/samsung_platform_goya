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

/* $Id: pipe_.h,v 1.7 2007/09/11 15:24:20 Arabidopsis Exp $ */
/* Declaration of popen and pclose */

#ifndef pipe__INCLUDED
#  define pipe__INCLUDED

#include "stdio_.h"

#ifdef __WIN32__
/*
 * MS Windows has popen and pclose in stdio.h, but under different names.
 * Unfortunately MSVC5 and 6 have a broken implementation of _popen, 
 * so we use own.  Our implementation only supports mode "wb".
 */
extern FILE *mswin_popen(const char *cmd, const char *mode);
#  define popen(cmd, mode) mswin_popen(cmd, mode)
/* #  define popen(cmd, mode) _popen(cmd, mode) */
#  define pclose(file) _pclose(file)
#else  /* !__WIN32__ */
/*
 * popen isn't POSIX-standard, so we declare it here.
 * Because of inconsistent (and sometimes incorrect) header files,
 * we must omit the argument list.  Unfortunately, this sometimes causes
 * more trouble than it cures.
 */
extern FILE *popen( /* const char *, const char * */ );
extern int pclose(FILE *);
#endif /* !__WIN32__ */

#endif /* pipe__INCLUDED */
