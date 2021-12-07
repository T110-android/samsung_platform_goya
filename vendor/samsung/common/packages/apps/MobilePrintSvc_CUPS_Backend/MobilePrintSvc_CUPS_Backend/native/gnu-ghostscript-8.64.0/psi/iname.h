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

/* $Id: iname.h,v 1.8 2007/09/11 15:23:59 Arabidopsis Exp $ */
/* Interpreter's name table interface */

#ifndef iname_INCLUDED
#  define iname_INCLUDED

#include "inames.h"

/*
 * This file defines those parts of the name table API that refer to the
 * interpreter's distinguished instance.  Procedures in this file begin
 * with name_.
 */

/* ---------------- Procedural interface ---------------- */


/* Get the allocator for the name table. */
#define name_memory(mem)\
  names_memory(mem->gs_lib_ctx->gs_name_table)

/*
 * Look up and/or enter a name in the name table.
 * See inames.h for the values of enterflag, and the possible return values.
 */
#define name_ref(mem, ptr, size, pnref, enterflag)\
  names_ref(mem->gs_lib_ctx->gs_name_table, ptr, size, pnref, enterflag)
#define name_string_ref(mem, pnref, psref)\
  names_string_ref(mem->gs_lib_ctx->gs_name_table, pnref, psref)
/*
 * name_enter_string calls name_ref with a (permanent) C string.
 */
#define name_enter_string(mem, str, pnref)\
  names_enter_string(mem->gs_lib_ctx->gs_name_table, str, pnref)
/*
 * name_from_string essentially implements cvn.
 * It always enters the name, and copies the executable attribute.
 */
#define name_from_string(mem, psref, pnref)\
  names_from_string(mem->gs_lib_ctx->gs_name_table, psref, pnref)

/* Compare two names for equality. */
#define name_eq(pnref1, pnref2)\
  names_eq(pnref1, pnref2)

/* Invalidate the value cache for a name. */
#define name_invalidate_value_cache(mem, pnref)\
  names_invalidate_value_cache(mem->gs_lib_ctx->gs_name_table, pnref)

/* Convert between names and indices. */
#define name_index(mem, pnref)		/* ref => index */\
  names_index(mem->gs_lib_ctx->gs_name_table, pnref)
#define name_index_ptr(mem, nidx)		/* index => name */\
  names_index_ptr(mem->gs_lib_ctx->gs_name_table, nidx)
#define name_index_ref(mem, nidx, pnref)	/* index => ref */\
  names_index_ref(mem->gs_lib_ctx->gs_name_table, nidx, pnref)

/* Get the index of the next valid name. */
/* The argument is 0 or a valid index. */
/* Return 0 if there are no more. */
#define name_next_valid_index(mem, nidx)\
  names_next_valid_index(mem->gs_lib_ctx->gs_name_table, nidx)

/* Mark a name for the garbage collector. */
/* Return true if this is a new mark. */
#define name_mark_index(mem, nidx)\
  names_mark_index(mem->gs_lib_ctx->gs_name_table, nidx)

/* Get the object (sub-table) containing a name. */
/* The garbage collector needs this so it can relocate pointers to names. */
#define name_ref_sub_table(mem, pnref)\
  names_ref_sub_table(mem->gs_lib_ctx->gs_name_table, pnref)

#endif /* iname_INCLUDED */
