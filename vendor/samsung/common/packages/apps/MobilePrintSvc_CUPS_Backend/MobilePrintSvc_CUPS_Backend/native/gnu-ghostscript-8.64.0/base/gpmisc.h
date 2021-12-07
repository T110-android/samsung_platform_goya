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

/* $Id: gpmisc.h,v 1.8 2007/09/11 15:24:36 Arabidopsis Exp $ */
/* Miscellaneous support for platform facilities */

#ifndef gpmisc_INCLUDED
#  define gpmisc_INCLUDED

/*
 * The facilities defined in this file and implemented in gpmisc.c are
 * generic utilities shared among multiple gp_ platform files.
 */

/*
 * Get the name of the directory for temporary files, if any.  Currently
 * this checks the TMPDIR and TEMP environment variables, in that order.
 * The return value and the setting of *ptr and *plen are as for gp_getenv.
 */
int gp_gettmpdir(char *ptr, int *plen);

/*
 * Open a temporary file, using O_EXCL and S_IRWXU to prevent race
 * conditions and symlink attacks.
 */
FILE *gp_fopentemp(const char *fname, const char *mode);
FILE *gp_fopentemp_64(const char *fname, const char *mode);

/*
 * Combine a file name with a prefix.
 * Concatenates two paths and reduce parent references and current 
 * directory references from the concatenation when possible.
 * The trailing zero byte is being added.
 */
gp_file_name_combine_result gp_file_name_combine_generic(const char *prefix, uint plen, 
	    const char *fname, uint flen, bool no_sibling, char *buffer, uint *blen);

/*
 * Reduces parent references and current directory references when possible.
 * The trailing zero byte is being added.
 */
gp_file_name_combine_result gp_file_name_reduce(const char *fname, uint flen, 
		char *buffer, uint *blen);

/* 
 * Answers whether a file name is absolute (starts from a root). 
 */
bool gp_file_name_is_absolute(const char *fname, uint flen);

/* 
 * Returns length of all starting parent references.
 */
uint gp_file_name_parents(const char *fname, uint flen);

/* 
 * Returns length of all starting cwd references.
 */
uint gp_file_name_cwds(const char *fname, uint flen);

#endif /* gpmisc_INCLUDED */
