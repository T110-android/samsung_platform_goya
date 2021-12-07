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

/* $Id: gsexit.h,v 1.9 2007/09/11 15:23:57 Arabidopsis Exp $ */
/* Declarations for exits */

#ifndef gsexit_INCLUDED
#  define gsexit_INCLUDED


/** The client must provide this.
 *  normally they do exit cleanup and error messaging
 *  without calling system exit() returning to the caller.
 */
int gs_to_exit(const gs_memory_t *mem, int exit_status);

/** some clients prefer this to return the postscript error code
 * to the caller otherwise the same as gs_to_exit()
 */
int gs_to_exit_with_code(const gs_memory_t *mem, int exit_status, int code);

/** The client must provide this.  
 * After possible cleanup it may call gp_do_exit() which calls exit() in a platform
 * independent way.  This is a fatal error so returning is not a good idea.
 */
void gs_abort(const gs_memory_t *mem);

#endif /* gsexit_INCLUDED */
