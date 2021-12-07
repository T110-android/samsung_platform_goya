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

/* $Id: zfontenum.c,v 1.8 2008/03/23 15:28:07 Arabidopsis Exp $ */

/* this is the ps interpreter interface to the native font
   enumeration code. it calls the platform-specific routines
   to obtain an additional set of entries that can be added
   to the Fontmap to reference fonts stored on the system.
 */

#include "memory_.h"
#include "string_.h"
#include <stdlib.h>
#include "ghost.h"
#include "oper.h"
#include "gsstruct.h"
#include "gsmalloc.h"
#include "ialloc.h"
#include "iname.h"
#include "iutil.h"
#include "store.h"
#include "gp.h"


typedef struct fontenum_s {
	char *fontname, *path;
	struct fontenum_s *next;
} fontenum_t;

/* .getnativefonts [ [<name> <path>] ... ] */
static int
z_fontenum(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    void *enum_state;
    int code = 0;
    int e,elements;
    char *fontname, *path;
    fontenum_t *r, *results;
    ref array;
    uint length;
    byte *string;
	
    enum_state = gp_enumerate_fonts_init(imemory);
    if (enum_state == NULL) {
      /* put false on the stack and return */
      push(1);
      make_bool(op, false);
      return code;
    }

    r = results = gs_malloc(imemory->non_gc_memory, 1, sizeof(fontenum_t), "fontenum list");
    elements = 0;
    while((code = gp_enumerate_fonts_next(enum_state, &fontname, &path )) > 0) {
	if (fontname == NULL || path == NULL) {
	    gp_enumerate_fonts_free(enum_state);
	    return_error(e_ioerror);
	}

	length = strlen(fontname) + 1;
	r->fontname = gs_malloc(imemory->non_gc_memory, length, 1, "native font name");
	memcpy(r->fontname, fontname, length);

	length = strlen(path) + 1;
	    r->path = gs_malloc(imemory->non_gc_memory, length, 1, "native font path");
	    memcpy(r->path, path, length);

	    r->next = gs_malloc(imemory->non_gc_memory, 1, sizeof(fontenum_t), "fontenum list");
	    r = r->next;
	    elements += 1;
	}
	
	gp_enumerate_fonts_free(enum_state);

	code = ialloc_ref_array(&array, a_all | icurrent_space, elements, "native fontmap");

	r = results;
	for (e = 0; e < elements; e++) {
	    ref mapping;
	
	    code = ialloc_ref_array(&mapping, a_all | icurrent_space, 2, "native font mapping");
		
	    length = strlen(r->fontname);
	    string = ialloc_string(length, "native font name");
	    if (string == NULL)
		return_error(e_VMerror);
	    memcpy(string, r->fontname, length);
	    make_string(&(mapping.value.refs[0]), a_all | icurrent_space, length, string);
	 	
	    length = strlen(r->path);
	    string = ialloc_string(length, "native font path");
	    if (string == NULL)
		return_error(e_VMerror);
	    memcpy(string, r->path, length);
	    make_string(&(mapping.value.refs[1]), a_all | icurrent_space, length, string);
	 	
	    ref_assign(&(array.value.refs[e]), &mapping);
	    results = r;
	    r = r->next;

	    gs_free(imemory->non_gc_memory, 
		    results->fontname, strlen(results->fontname) + 1, 1, "native font name");
	    gs_free(imemory->non_gc_memory, 
		    results->path, strlen(results->path) + 1, 1, "native font path");
	    gs_free(imemory->non_gc_memory, 
		    results, 1, sizeof(fontenum_t), "fontenum list");
	}

    push(2);   
    ref_assign(op-1, &array);
    make_bool(op, true);
	
    return code;
}


/* Match the above routines to their postscript filter names.
   This is how our static routines get called externally. */
const op_def zfontenum_op_defs[] = {
    {"0.getnativefonts", z_fontenum},
    op_def_end(0)
};
