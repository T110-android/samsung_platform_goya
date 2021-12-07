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

/* $Id: gxdevrop.h,v 1.7 2007/09/11 15:24:20 Arabidopsis Exp $ */
/* Extension of gxdevice.h for RasterOp */

#ifndef gxdevrop_INCLUDED
#  define gxdevrop_INCLUDED

/* Define an unaligned implementation of [strip_]copy_rop. */
dev_proc_copy_rop(gx_copy_rop_unaligned);
dev_proc_strip_copy_rop(gx_strip_copy_rop_unaligned);

#endif /* gxdevrop_INCLUDED */
