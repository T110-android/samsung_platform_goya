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

/* $Id: igcstr.h,v 1.8 2007/09/11 15:23:54 Arabidopsis Exp $ */
/* Internal interface to string garbage collector */

#ifndef igcstr_INCLUDED
#  define igcstr_INCLUDED

/* Exported by ilocate.c for igcstr.c */
chunk_t *gc_locate(const void *, gc_state_t *);

/* Exported by igcstr.c for igc.c */
void gc_strings_set_marks(chunk_t *, bool);
bool gc_string_mark(const byte *, uint, bool, gc_state_t *);
void gc_strings_clear_reloc(chunk_t *);
void gc_strings_set_reloc(chunk_t *);
void gc_strings_compact(chunk_t *);
string_proc_reloc(igc_reloc_string);
const_string_proc_reloc(igc_reloc_const_string);
param_string_proc_reloc(igc_reloc_param_string);

#endif /* igcstr_INCLUDED */
