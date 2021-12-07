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

/* $Id: shc.c,v 1.7 2007/09/11 15:23:52 Arabidopsis Exp $ */
/* Support code for shc.h */
#include "std.h"
#include "scommon.h"
#include "shc.h"

/* ------ Encoding ------ */

/* Empty the 1-word buffer onto the output stream. */
/* q has already been incremented. */
void
hc_put_code_proc(bool reverse_bits, byte * q, uint cw)
{
#define cb(n) ((byte)(cw >> (n * 8)))
    if (reverse_bits) {
#if hc_bits_size > 16
	q[-3] = byte_reverse_bits[cb(3)];
	q[-2] = byte_reverse_bits[cb(2)];
#endif
	q[-1] = byte_reverse_bits[cb(1)];
	q[0] = byte_reverse_bits[cb(0)];
    } else {
#if hc_bits_size > 16
	q[-3] = cb(3);
	q[-2] = cb(2);
#endif
	q[-1] = cb(1);
	q[0] = cb(0);
    }
#undef cb
}

/* Put out any final bytes. */
/* Note that this does a store_state, but not a load_state. */
byte *
hc_put_last_bits_proc(stream_hc_state * ss, byte * q, uint bits, int bits_left)
{
    while (bits_left < hc_bits_size) {
	byte c = (byte) (bits >> (hc_bits_size - 8));

	if (ss->FirstBitLowOrder)
	    c = byte_reverse_bits[c];
	*++q = c;
	bits <<= 8;
	bits_left += 8;
    }
    ss->bits = bits;
    ss->bits_left = bits_left;
    return q;
}
