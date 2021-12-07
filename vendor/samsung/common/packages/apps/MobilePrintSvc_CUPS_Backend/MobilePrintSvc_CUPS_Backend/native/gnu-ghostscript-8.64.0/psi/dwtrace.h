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

/* $Id: dwtrace.h,v 1.9 2009/04/19 13:54:26 Arabidopsis Exp $ */
/* The interface of Graphical trace server for Windows */

#ifndef dwtrace_INCLUDED
#  define dwtrace_INCLUDED

extern struct vd_trace_interface_s visual_tracer;
struct vd_trace_interface_s *visual_tracer_init(void);
void visual_tracer_close(void);

#endif /* dwtrace_INCLUDED */
