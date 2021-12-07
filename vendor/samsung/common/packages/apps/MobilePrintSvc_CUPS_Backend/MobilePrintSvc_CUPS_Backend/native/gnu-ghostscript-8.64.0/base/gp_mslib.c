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

/* $Id: gp_mslib.c,v 1.8 2007/09/11 15:24:39 Arabidopsis Exp $ */
/*
 * Microsoft Windows platform support for Graphics Library
 *
 * This file implements functions that differ between the graphics
 * library and the interpreter.
 */

/* ------ Process message loop ------ */
/* 
 * Check messages and interrupts; return true if interrupted.
 * This is called frequently - it must be quick!
 */
#ifdef CHECK_INTERRUPTS

#include "gx.h"

int
gp_check_interrupts(const gs_memory_t *mem)
{
    return 0;
}
#endif
