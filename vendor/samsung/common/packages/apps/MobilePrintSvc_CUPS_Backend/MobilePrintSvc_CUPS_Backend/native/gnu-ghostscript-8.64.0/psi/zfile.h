/* Copyright (C) 2008 Artifex Software, Inc.
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

/* $Id: zfile.h,v 1.1 2009/04/19 13:54:23 Arabidopsis Exp $ */
/* Export zopen_file() for zfrsd.c */

#ifndef zfile_INCLUDED
#  define zfile_INCLUDED

int zopen_file(i_ctx_t *i_ctx_p, const gs_parsed_file_name_t *pfn,
           const char *file_access, stream **ps, gs_memory_t *mem);

#endif
