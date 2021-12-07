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

/* $Id: gshsb.h,v 1.7 2007/09/11 15:24:28 Arabidopsis Exp $ */
/* Client interface to HSB color routines */

#ifndef gshsb_INCLUDED
#  define gshsb_INCLUDED

int gs_sethsbcolor(gs_state *, floatp, floatp, floatp),
    gs_currenthsbcolor(const gs_state *, float[3]);

#endif /* gshsb_INCLUDED */
