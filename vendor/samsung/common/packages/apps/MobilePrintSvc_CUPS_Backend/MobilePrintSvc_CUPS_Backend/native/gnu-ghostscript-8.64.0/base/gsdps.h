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

/* $Id: gsdps.h,v 1.7 2007/09/11 15:24:20 Arabidopsis Exp $ */
/* Client interface to Display PostScript facilities. */

#ifndef gsdps_INCLUDED
#  define gsdps_INCLUDED

/* Device-source images */
#include "gsiparm2.h"

/* View clipping */
int gs_initviewclip(gs_state *);
int gs_eoviewclip(gs_state *);
int gs_viewclip(gs_state *);
int gs_viewclippath(gs_state *);

#endif /* gsdps_INCLUDED */
