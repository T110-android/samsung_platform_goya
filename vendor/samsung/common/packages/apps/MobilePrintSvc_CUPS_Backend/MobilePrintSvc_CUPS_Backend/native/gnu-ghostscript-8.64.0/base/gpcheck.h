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

/* $Id: gpcheck.h,v 1.9 2007/09/11 15:23:55 Arabidopsis Exp $ */
/* Interrupt check interface */

#ifndef gpcheck_INCLUDED
#  define gpcheck_INCLUDED

/*
 * On some platforms, the interpreter must check periodically for user-
 * initiated actions.  (Eventually, this may be extended to all platforms,
 * to handle multi-tasking through the 'context' facility.)  Routines that
 * run for a long time must periodically call gp_check_interrupts(), and
 * if it returns true, must clean up whatever they are doing and return an
 * e_interrupted (or gs_error_interrupted) exceptional condition.
 * The return_if_interrupt macro provides a convenient way to do this.
 *
 * On platforms that require an interrupt check, the makefile defines
 * a symbol CHECK_INTERRUPTS.  Currently this is only the Microsoft
 * Windows platform.
 */
int gs_return_check_interrupt(const gs_memory_t *mem, int code);

#ifdef CHECK_INTERRUPTS
int gp_check_interrupts(const gs_memory_t *mem);
#  define process_interrupts(mem) discard(gp_check_interrupts(mem))
#  define return_if_interrupt(mem)\
    { int icode_ = gp_check_interrupts(mem);	\
      if ( icode_ )\
	return gs_note_error((icode_ > 0 ? gs_error_interrupt : icode_));\
    }
#  define return_check_interrupt(mem, code)	\
    return gs_return_check_interrupt(mem, code)
#  define set_code_on_interrupt(mem, pcode)	\
    if (*(pcode) == 0)\
     *(pcode) = (gp_check_interrupts(mem) != 0) ? gs_error_interrupt : 0;
#else
#  define gp_check_interrupts(mem) 0
#  define process_interrupts(mem) DO_NOTHING
#  define return_if_interrupt(mem)	DO_NOTHING
#  define return_check_interrupt(mem, code)	\
    return (code)
#  define set_code_on_interrupt(mem, code) DO_NOTHING
#endif

#endif /* gpcheck_INCLUDED */
