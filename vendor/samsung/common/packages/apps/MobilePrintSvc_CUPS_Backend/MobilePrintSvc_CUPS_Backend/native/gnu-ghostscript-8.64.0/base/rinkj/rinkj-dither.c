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

/*$Id: rinkj-dither.c,v 1.4 2007/09/11 15:25:09 Arabidopsis Exp $ */
/* The dither object abstraction within the Rinkj driver. */

#include "rinkj-dither.h"

void
rinkj_dither_line (RinkjDither *self, unsigned char *dst, const unsigned char *src)
{
  self->dither_line (self, dst, src);
}

void
rinkj_dither_close (RinkjDither *self)
{
  self->close (self);
}
