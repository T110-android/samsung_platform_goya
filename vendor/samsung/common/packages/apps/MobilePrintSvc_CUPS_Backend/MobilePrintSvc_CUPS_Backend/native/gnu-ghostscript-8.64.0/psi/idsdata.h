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

/* $Id: idsdata.h,v 1.7 2007/09/11 15:24:17 Arabidopsis Exp $ */
/* Generic dictionary stack structure definition */

#ifndef idsdata_INCLUDED
#  define idsdata_INCLUDED

#include "isdata.h"

/* Define the dictionary stack structure. */
#ifndef dict_stack_DEFINED
#  define dict_stack_DEFINED
typedef struct dict_stack_s dict_stack_t;
#endif
struct dict_stack_s {

    ref_stack_t stack;		/* the actual stack of dictionaries */

/*
 * Switching between Level 1 and Level 2 involves inserting and removing
 * globaldict on the dictionary stack.  Instead of truly inserting and
 * removing entries, we replace globaldict by a copy of systemdict in
 * Level 1 mode.  min_dstack_size, the minimum number of entries, does not
 * change depending on language level; the countdictstack and dictstack
 * operators must take this into account.
 */
    uint min_size;		/* size of stack after clearing */

    int userdict_index;		/* index of userdict on stack */

/*
 * Cache a value for fast checking of def operations.
 * If the top entry on the dictionary stack is a writable dictionary,
 * dsspace is the space of the dictionary; if it is a non-writable
 * dictionary, dsspace = -1.  Then def is legal precisely if
 * r_space(pvalue) <= dsspace.  Note that in order for this trick to work,
 * the result of r_space must be a signed integer; some compilers treat
 * enums as unsigned, probably in violation of the ANSI standard.
 */
    int def_space;

/*
 * Cache values for fast name lookup.  If the top entry on the dictionary
 * stack is a readable dictionary with packed keys, dtop_keys, dtop_npairs,
 * and dtop_values are keys.value.packed, npairs, and values.value.refs
 * for that dictionary; otherwise, these variables point to a dummy
 * empty dictionary.
 */
    const ref_packed *top_keys;
    uint top_npairs;
    ref *top_values;

/*
 * Cache a copy of the bottom entry on the stack, which is never deleted.
 */
    ref system_dict;

};

/*
 * The top-entry pointers are recomputed after garbage collection, so we
 * don't declare them as pointers.
 */
#define public_st_dict_stack()	/* in interp.c */\
  gs_public_st_suffix_add0(st_dict_stack, dict_stack_t, "dict_stack_t",\
    dict_stack_enum_ptrs, dict_stack_reloc_ptrs, st_ref_stack)
#define st_dict_stack_num_ptrs st_ref_stack_num_ptrs

#endif /* idsdata_INCLUDED */
