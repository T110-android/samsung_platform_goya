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
/* Portions Copyright (C) 1994-2000 Ghostgum Software Pty Ltd.  All rights reserved. */


/* $Id: gsdll.c,v 1.10 2008/03/23 15:27:37 Arabidopsis Exp $ */
/* Dynamic Link Library interface for OS/2 and MS-Windows Ghostscript */
/* front end to gs.c */

/* This has been reimplemented to call the new DLL interface in iapi.h */


#ifdef _Windows
#include <windows.h>
#endif
#ifdef __OS2__
#define INCL_DOS
#define INCL_WIN
#include <os2.h>
#endif

#include "stdpre.h"
#include "iapi.h"	/* Ghostscript interpreter public interface */
#include "string_.h"
#include "ierrors.h"
#include "gscdefs.h"
#include "gstypes.h"
#include "iref.h"
#include "iminst.h"
#include "imain.h"

#include "gsdll.h"	/* old DLL public interface */

/* MacGSView still requires that hwnd be exported
   through the old dll interface. We do that here,
   but expect to remove it when that client has been
   ported to the gsapi interface. */
#ifdef __MACOS__
extern HWND hwndtext;
#endif

/****** SINGLE-INSTANCE HACK ******/
/* GLOBAL WARNING */
GSDLL_CALLBACK pgsdll_callback = NULL;	/* callback for messages and stdio to caller */

static gs_main_instance *pgs_minst = NULL;


/****** SINGLE-INSTANCE HACK ******/


/* local functions */
static int GSDLLCALL gsdll_old_stdin(void *caller_handle, char *buf, int len);
static int GSDLLCALL gsdll_old_stdout(void *caller_handle, const char *str, int len);
static int GSDLLCALL gsdll_old_stderr(void *caller_handle, const char *str, int len);
static int GSDLLCALL gsdll_old_poll(void *caller_handle);


/* ---------- DLL exported functions ---------- */

/* arguments are:
 * 1. callback function for stdio and for notification of 
 *   sync_output, output_page and resize events
 * 2. window handle, used as parent.  Use NULL if you have no window.
 * 3. argc
 * 4. argv
 */
int GSDLLEXPORT GSDLLAPI
gsdll_init(GSDLL_CALLBACK callback, HWND hwnd, int argc, char * argv[])
{
    int code;

    if ((code = gsapi_new_instance(&pgs_minst, (void *)1)) < 0)
	return -1;

    gsapi_set_stdio(pgs_minst, 
	gsdll_old_stdin, gsdll_old_stdout, gsdll_old_stderr);
    gsapi_set_poll(pgs_minst, gsdll_old_poll);
    /* ignore hwnd */

/* rest of MacGSView compatibilty hack */
#ifdef __MACOS__
	hwndtext=hwnd;
#endif

/****** SINGLE-INSTANCE HACK ******/
    pgsdll_callback = callback;
/****** SINGLE-INSTANCE HACK ******/

    code = gsapi_init_with_args(pgs_minst, argc, argv);
    if (code == e_Quit) {
	gsapi_exit(pgs_minst);
	return GSDLL_INIT_QUIT;
    }
    return code;
}

/* if return value < 0, then error occured and caller should call */
/* gsdll_exit, then unload library */
int GSDLLEXPORT GSDLLAPI
gsdll_execute_begin(void)
{
    int exit_code;
    return gsapi_run_string_begin(pgs_minst, 0, &exit_code);
}

/* if return value < 0, then error occured and caller should call */
/* gsdll_execute_end, then gsdll_exit, then unload library */
int GSDLLEXPORT GSDLLAPI
gsdll_execute_cont(const char * str, int len)
{
    int exit_code;
    int code = gsapi_run_string_continue(pgs_minst, str, len, 
	0, &exit_code);
    if (code == e_NeedInput)
	code = 0;		/* this is not an error */
    return code;
}

/* if return value < 0, then error occured and caller should call */
/* gsdll_exit, then unload library */
int GSDLLEXPORT GSDLLAPI
gsdll_execute_end(void)
{
    int exit_code;
    return gsapi_run_string_end(pgs_minst, 0, &exit_code);
}

int GSDLLEXPORT GSDLLAPI
gsdll_exit(void)
{
    int code = gsapi_exit(pgs_minst);

    gsapi_delete_instance(pgs_minst);
    return code;
}

/* Return revision numbers and strings of Ghostscript. */
/* Used for determining if wrong GSDLL loaded. */
/* This may be called before any other function. */
int GSDLLEXPORT GSDLLAPI
gsdll_revision(const char ** product, const char ** copyright,
	       long * revision, long * revisiondate)
{
    if (product)
	*product = gs_product;
    if (copyright)
	*copyright = gs_copyright;
    if (revision)
	*revision = gs_revision;
    if (revisiondate)
	*revisiondate = gs_revisiondate;
    return 0;
}


static int GSDLLCALL
gsdll_old_stdin(void *caller_handle, char *buf, int len)
{
    return (*pgsdll_callback)(GSDLL_STDIN, buf, len);
}
static int GSDLLCALL
gsdll_old_stdout(void *caller_handle, const char *str, int len)
{
    return (*pgsdll_callback)(GSDLL_STDOUT, (char *)str, len);
}

static int GSDLLCALL
gsdll_old_stderr(void *caller_handle, const char *str, int len)
{
    return (*pgsdll_callback)(GSDLL_STDOUT, (char *)str, len);
}

static int GSDLLCALL
gsdll_old_poll(void *caller_handle)
{
    return (*pgsdll_callback)(GSDLL_POLL, NULL, 0);
}

/* end gsdll.c */
