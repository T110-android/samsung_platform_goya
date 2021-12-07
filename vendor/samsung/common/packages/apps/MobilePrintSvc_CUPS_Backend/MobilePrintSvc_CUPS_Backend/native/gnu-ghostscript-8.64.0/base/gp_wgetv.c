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

/* $Id: gp_wgetv.c,v 1.9 2007/09/11 15:24:10 Arabidopsis Exp $ */
/* MS Windows implementation of gp_getenv */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>		/* for getenv */
#include <string.h>
#include "gscdefs.h"		/* for gs_productfamily and gs_revision */

/* prototypes */
int gp_getenv_registry(HKEY hkeyroot, const char *key, const char *name, 
    char *ptr, int *plen);

/* ------ Environment variables ------ */

/* Get the value of an environment variable.  See gp.h for details. */
int 
gp_getenv(const char *name, char *ptr, int *plen)
{
    const char *str = getenv(name);

    if (str) {
	int len = strlen(str);

	if (len < *plen) {
	    /* string fits */
	    strcpy(ptr, str);
	    *plen = len + 1;
	    return 0;
	}
	/* string doesn't fit */
	*plen = len + 1;
	return -1;
    }
    /* environment variable was not found */

#ifdef __WIN32__
    {
	/* If using Win32, look in the registry for a value with
	 * the given name.  The registry value will be under the key
	 * HKEY_CURRENT_USER\Software\GNU Ghostscript\N.NN.N
	 * or if that fails under the key
	 * HKEY_LOCAL_MACHINE\Software\GNU Ghostscript\N.NN.N
	 * where "GNU Ghostscript" is actually gs_productfamily
	 * and N.NN.N is obtained from gs_revision.
	 */
	DWORD version = GetVersion();

	if (!(((HIWORD(version) & 0x8000) != 0)
	      && ((HIWORD(version) & 0x4000) == 0))) {
	    /* not Win32s */
	    int code;
	    char key[256];
	    char dotversion[16];
	    
	    sprintf(dotversion, "%d.%02d", (int)(gs_revision / 100),
		    (int)(gs_revision % 100));
	    sprintf(key, "Software\\%s\\%s", gs_productfamily, dotversion);

	    code = gp_getenv_registry(HKEY_CURRENT_USER, key, name, ptr, plen);
	    if ( code <= 0 )
		return code;	/* found it */

	    code = gp_getenv_registry(HKEY_LOCAL_MACHINE, key, name, ptr, plen);
	    if ( code <= 0 )
		return code;	/* found it */
	}
    }
#endif

    /* nothing found at all */

    if (*plen > 0)
	*ptr = 0;
    *plen = 1;
    return 1;
}


/*
 * Get a named registry value.
 * Key = hkeyroot\\key, named value = name.
 * name, ptr, plen and return values are the same as in gp_getenv();
 */

int 
gp_getenv_registry(HKEY hkeyroot, const char *key, const char *name, 
    char *ptr, int *plen)
{
    HKEY hkey;
    DWORD cbData, keytype;
    BYTE b;
    LONG rc;
    BYTE *bptr = (BYTE *)ptr;

    if (RegOpenKeyEx(hkeyroot, key, 0, KEY_READ, &hkey)
	== ERROR_SUCCESS) {
	keytype = REG_SZ;
	cbData = *plen;
	if (bptr == (char *)NULL)
	    bptr = &b;	/* Registry API won't return ERROR_MORE_DATA */
			/* if ptr is NULL */
	rc = RegQueryValueEx(hkey, (char *)name, 0, &keytype, bptr, &cbData);
	RegCloseKey(hkey);
	if (rc == ERROR_SUCCESS) {
	    *plen = cbData;
	    return 0;	/* found environment variable and copied it */
	} else if (rc == ERROR_MORE_DATA) {
	    /* buffer wasn't large enough */
	    *plen = cbData;
	    return -1;
	}
    }
    return 1;	/* not found */
}
