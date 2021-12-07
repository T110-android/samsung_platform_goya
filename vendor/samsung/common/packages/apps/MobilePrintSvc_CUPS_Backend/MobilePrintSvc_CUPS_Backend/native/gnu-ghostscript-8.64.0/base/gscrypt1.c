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

/* $Id: gscrypt1.c,v 1.7 2007/09/11 15:23:47 Arabidopsis Exp $ */
/* Adobe Type 1 encryption/decryption. */
#include "stdpre.h"
#include "gstypes.h"
#include "gscrypt1.h"

/* Encrypt a string. */
int
gs_type1_encrypt(byte * dest, const byte * src, uint len, crypt_state * pstate)
{
    crypt_state state = *pstate;
    const byte *from = src;
    byte *to = dest;
    uint count = len;

    while (count) {
	encrypt_next(*from, state, *to);
	from++, to++, count--;
    }
    *pstate = state;
    return 0;
}
/* Decrypt a string. */
int
gs_type1_decrypt(byte * dest, const byte * src, uint len, crypt_state * pstate)
{
    crypt_state state = *pstate;
    const byte *from = src;
    byte *to = dest;
    uint count = len;

    while (count) {
	/* If from == to, we can't use the obvious */
	/*      decrypt_next(*from, state, *to);        */
	byte ch = *from++;

	decrypt_next(ch, state, *to);
	to++, count--;
    }
    *pstate = state;
    return 0;
}
