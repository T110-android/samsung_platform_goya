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

/* $Id: idebug.h,v 1.8 2007/09/11 15:24:31 Arabidopsis Exp $ */
/* Prototypes for debugging procedures in idebug.c */

#ifndef idebug_INCLUDED
#  define idebug_INCLUDED

/* Print individual values. */
void debug_print_name(const gs_memory_t *mem, const ref *);
void debug_print_name_index(const gs_memory_t *mem, uint /*name_index_t*/);
void debug_print_ref(const gs_memory_t *mem, const ref *);
void debug_print_ref_packed(const gs_memory_t *mem, const ref_packed *);

/* Dump regions of memory. */
void debug_dump_one_ref(const gs_memory_t *mem, const ref *);
void debug_dump_refs(const gs_memory_t *mem, 
		     const ref * from, uint size, const char *msg);
void debug_dump_array(const gs_memory_t *mem, const ref * array);

/* Dump a stack.  Using this requires istack.h. */
#ifndef ref_stack_DEFINED
typedef struct ref_stack_s ref_stack_t;	/* also defined in isdata.h */
#  define ref_stack_DEFINED
#endif
void debug_dump_stack(const gs_memory_t *mem, 
		      const ref_stack_t * pstack, const char *msg);

#endif /* idebug_INCLUDED */
