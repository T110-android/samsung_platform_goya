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

/* $Id: idosave.h,v 1.8 2007/09/11 15:24:34 Arabidopsis Exp $ */
/* Supporting procedures for 'save' recording. */

#ifndef idosave_INCLUDED
#  define idosave_INCLUDED

/*
 * Save a change that must be undone by restore.  We have to pass the
 * pointer to the containing object to alloc_save_change for two reasons:
 *
 *      - We need to know which VM the containing object is in, so we can
 * know on which chain of saved changes to put the new change.
 *
 *      - We need to know whether the object is an array of refs (which
 * includes dictionaries) or a struct, so we can properly trace and
 * relocate the pointer to it from the change record during garbage
 * collection.
 */
int alloc_save_change(gs_dual_memory_t *dmem, const ref *pcont,
		      ref_packed *ptr, client_name_t cname);
int alloc_save_change_in(gs_ref_memory_t *mem, const ref *pcont,
			 ref_packed *ptr, client_name_t cname);
/* Remove an AC_OFFSET_ALLOCATED element. */
void alloc_save_remove(gs_ref_memory_t *mem, ref_packed *obj, client_name_t cname);
/* Allocate a structure for recording an allocation event. */
int alloc_save_change_alloc(gs_ref_memory_t *mem, client_name_t cname, ref_packed ***ppr);

#endif /* idosave_INCLUDED */
