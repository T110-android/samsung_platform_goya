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

/* $Id: gp_macpoll.c,v 1.7 2007/09/11 15:24:32 Arabidopsis Exp $ */
/*
 * Macintosh platform polling support for Ghostscript.
 *
 */

#ifndef __CARBON__
#include <Timer.h>
#else
#include <Carbon.h>
#endif

#include "gx.h"
#include "gp.h"
#include "gsdll.h"
#include "gpcheck.h"
#include "iapi.h"
#include "iref.h"
#include "iminst.h"
#include "imain.h"

#ifdef CHECK_INTERRUPTS

extern HWND hwndtext;

/* ------ Process message loop ------ */
/* 
 * Check messages and interrupts; return true if interrupted.
 * This is called frequently - it must be quick!
 */
int gp_check_interrupts(const gs_memory_t *mem)
{
	/* static variables need to go away for thread safety */
	static unsigned long	lastYieldTicks = 0;
	int iRetVal = 0;
	
	if ((TickCount() - lastYieldTicks) > 2) {
	    lastYieldTicks = TickCount();
	    if (pgsdll_callback) {
		/* WARNING: The use of the old gsdll interface is deprecated. 
		 * The caller should use the newer gsapi_set_poll.
		 * If the caller needs access to "hwndtext", it should do 
		 * this via caller_handle which is passed to poll_fn.
		 */
		/* the hwnd parameter which is submitted in gsdll_init 
		 * to the DLL is returned in every gsdll_poll message 
		 * in the count parameter
		 */
		iRetVal = (*pgsdll_callback)(GSDLL_POLL, 0, (long) hwndtext);
	    } else {
	    	if (mem == NULL) {
	    		/* this is not thread safe */
	    		mem = gs_lib_ctx_get_non_gc_memory_t();
	    	}
		if (mem && mem->gs_lib_ctx && mem->gs_lib_ctx->poll_fn)
		    iRetVal = (*mem->gs_lib_ctx->poll_fn)(mem->gs_lib_ctx->caller_handle);
	    }
	}
	return iRetVal;
}
#endif
