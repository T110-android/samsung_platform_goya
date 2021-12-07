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

/* $Id: ifwpred.h,v 1.7 2007/09/11 15:24:13 Arabidopsis Exp $ */
/* filter_read_predictor prototype */

#ifndef ifwpred_INCLUDED
#  define ifwpred_INCLUDED

/* Exported by zfilter2.c for zfzlib.c */
int filter_write_predictor(i_ctx_t *i_ctx_p, int npop,
			   const stream_template * template,
			   stream_state * st);

#endif /* ifwpred_INCLUDED */
