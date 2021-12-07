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

/* $Id: gxttfb.h,v 1.6 2007/09/11 15:24:29 Arabidopsis Exp $ */
/* A bridge to True Type interpreter. */

#ifndef gxttfb_INCLUDED
#  define gxttfb_INCLUDED

#include "ttfoutl.h"

#ifndef gx_ttfReader_DEFINED
#  define gx_ttfReader_DEFINED
typedef struct gx_ttfReader_s gx_ttfReader;
#endif

#ifndef gs_font_type42_DEFINED
#  define gs_font_type42_DEFINED
typedef struct gs_font_type42_s gs_font_type42;
#endif

struct gx_ttfReader_s {
    ttfReader super;
    int pos;
    bool error;
    int extra_glyph_index;
    gs_font_type42 *pfont;
    gs_memory_t *memory;
    gs_glyph_data_t glyph_data;
    /*  When TT interpreter is invoked, a font and the TT interpreter instance
	may use different memory allocators (local and global correspondingly).
	Since we don't want to change Free Type function prototypes, 
	we place the gx_ttfReader instance into the global memory,
	to provide an access to it through TExecution_Context.
	Due to that, the fields 'pfont' and 'glyph_data' may contain pointers from global 
	to local memory. They must be NULL when a garbager is invoked.
	We reset them whan the TT interpreter exits.
     */
};

gx_ttfReader *gx_ttfReader__create(gs_memory_t *mem);
void gx_ttfReader__destroy(gx_ttfReader *this);
void gx_ttfReader__set_font(gx_ttfReader *this, gs_font_type42 *pfont);
ttfFont *ttfFont__create(gs_font_dir *dir);
void ttfFont__destroy(ttfFont *this, gs_font_dir *dir);
int ttfFont__Open_aux(ttfFont *this, ttfInterpreter *tti, gx_ttfReader *r, gs_font_type42 *pfont,
    	       const gs_matrix * char_tm, const gs_log2_scale_point *log2_scale,
	       bool design_grid);
int gx_ttf_outline(ttfFont *ttf, gx_ttfReader *r, gs_font_type42 *pfont, int glyph_index, 
	const gs_matrix *m, const gs_log2_scale_point * pscale, 
	gx_path *path, bool grid_fit);

#endif /* gxttfb_INCLUDED */
