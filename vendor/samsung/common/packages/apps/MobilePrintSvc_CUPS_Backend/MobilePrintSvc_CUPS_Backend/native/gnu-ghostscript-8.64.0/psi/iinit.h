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

/* $Id: iinit.h,v 1.8 2008/03/23 15:27:57 Arabidopsis Exp $ */
/* (Internal) interface to iinit.c */

#ifndef iinit_INCLUDED
#  define iinit_INCLUDED

/*
 * Declare initialization procedures exported by iinit.c for imain.c.
 * These must be executed in the order they are declared below.
 */
int obj_init(i_ctx_t **, gs_dual_memory_t *);
int zop_init(i_ctx_t *);
int op_init(i_ctx_t *);
#ifdef DEBUG_TRACE_PS_OPERATORS
const char *op_get_name_string(op_proc_t opproc);
#endif

/*
 * Test whether there are any Level 2 operators in the executable.
 * (This is different from the language level in which the interpreter is
 * actually running: it is only tested during initialization.)
 */
bool gs_have_level2(void);

#endif /* iinit_INCLUDED */
