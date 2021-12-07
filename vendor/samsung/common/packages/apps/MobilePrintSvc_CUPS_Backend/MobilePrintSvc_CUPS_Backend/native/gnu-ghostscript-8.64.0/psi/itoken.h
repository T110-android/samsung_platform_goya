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

/* $Id: itoken.h,v 1.8 2008/03/23 15:27:46 Arabidopsis Exp $ */
/* Interface to exported procedures in ztoken.c */

#ifndef itoken_INCLUDED
#  define itoken_INCLUDED

/*
 * Continue after handling a procedure stream refill or other callout
 * while reading tokens in the interpreter.
 */
int ztokenexec_continue(i_ctx_t *i_ctx_p);

/*
 * Handle a scan_Comment or scan_DSC_Comment return from scan_token.
 */
#ifndef scanner_state_DEFINED
#  define scanner_state_DEFINED
typedef struct scanner_state_s scanner_state;
#endif
int ztoken_handle_comment(i_ctx_t *i_ctx_p,
			  scanner_state *sstate, const ref *ptoken,
			  int scan_code, bool save, bool push_file,
			  op_proc_t cont);

/*
 * Update the cached scanner_options in the context state after doing a
 * setuserparams.  (We might move this procedure somewhere else eventually.)
 */
int ztoken_scanner_options(const ref *upref, int old_options);
/*
 * Get the value for a scanner option.
 * return -1 if no such option, 1/0 for on/off and option's name in *pname as a C string
 */
int ztoken_get_scanner_option(const ref *psref, int options, const char **pname);

#endif /* itoken_INCLUDED */
