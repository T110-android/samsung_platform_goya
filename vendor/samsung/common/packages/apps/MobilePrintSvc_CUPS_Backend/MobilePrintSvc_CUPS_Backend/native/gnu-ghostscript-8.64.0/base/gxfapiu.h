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

/* $Id: gxfapiu.h,v 1.3 2007/09/11 15:24:23 Arabidopsis Exp $ */
/* Font API support : UFST callback dispatch. */

#ifndef gxfapiu_INCLUDED
#define gxfapiu_INCLUDED

#include "gp.h"

/* Set UFST callbacks. */
/* Warning : the language switch progect doesn't guarantee
   that this function is called when switching
   to another interpreter. Therefore each interpreter must take
   care for its own callback methods before they 
   may be called by UFST.
 */
 /* Warning : this function may cause a reentrancy problem
    due to a modification of static variables.
    Nevertheless this problem isn't important in a
    sngle interpreter build because the values
    really change on the first demand only.
    See also a comment in gs_fapiufst_finit.
  */
void gx_set_UFST_Callbacks(LPUB8 (*p_PCLEO_charptr)(FSP LPUB8 pfont_hdr, UW16  sym_code),
                           LPUB8 (*p_PCLchId2ptr)(FSP UW16 chId),
                           LPUB8 (*p_PCLglyphID2Ptr)(FSP UW16 glyphID));

void gx_reset_UFST_Callbacks(void);

typedef struct fco_list_elem_s fco_list_elem;
struct fco_list_elem_s {
    int open_count;
    SW16 fcHandle;
    char *file_path;
    fco_list_elem *next;
};

/* Access to the static FCO list for the language switching project : */
/* For the language switch : */
UW16 gx_UFST_open_static_fco(const char *font_file_path, SW16 *result_fcHandle);
UW16 gx_UFST_close_static_fco(SW16 fcHandle);
/* close all open FCO's */
void gx_UFST_close_static_fcos(void);
SW16 gx_UFST_find_fco_handle_by_name(const char *font_file_path);

/* For fapiufst.c : */
fco_list_elem *gx_UFST_find_static_fco(const char *font_file_path);
fco_list_elem *gx_UFST_find_static_fco_handle(SW16 fcHandle);

int gx_UFST_init(const UB8 *ufst_root_dir);

int gx_UFST_fini(void);

#endif /* gxfapiu_INCLUDED */
