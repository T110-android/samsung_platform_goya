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

/* $Id: gsbittab.h,v 1.7 2007/09/11 15:24:13 Arabidopsis Exp $ */
/* Interface to tables for bit operations */

#ifndef gsbittab_INCLUDED
#  define gsbittab_INCLUDED

/*
 * Generate tables for transforming 2, 4, 6, or 8 bits.
 */
#define btab2_(v0,v2,v1)\
  v0,v1+v0,v2+v0,v2+v1+v0
#define bit_table_2(v0,v2,v1) btab2_(v0,v2,v1)
#define btab4_(v0,v8,v4,v2,v1)\
  btab2_(v0,v2,v1), btab2_(v4+v0,v2,v1),\
  btab2_(v8+v0,v2,v1), btab2_(v8+v4+v0,v2,v1)
#define bit_table_4(v0,v8,v4,v2,v1) btab4_(v0,v8,v4,v2,v1)
#define btab6_(v0,v20,v10,v8,v4,v2,v1)\
  btab4_(v0,v8,v4,v2,v1), btab4_(v10+v0,v8,v4,v2,v1),\
  btab4_(v20+v0,v8,v4,v2,v1), btab4_(v20+v10+v0,v8,v4,v2,v1)
#define bit_table_6(v0,v20,v10,v8,v4,v2,v1) btab6_(v0,v20,v10,v8,v4,v2,v1)
#define bit_table_8(v0,v80,v40,v20,v10,v8,v4,v2,v1)\
  btab6_(v0,v20,v10,v8,v4,v2,v1), btab6_(v40+v0,v20,v10,v8,v4,v2,v1),\
  btab6_(v80+v0,v20,v10,v8,v4,v2,v1), btab6_(v80+v40+v0,v20,v10,v8,v4,v2,v1)

/*
 * byte_reverse_bits[B] = the byte B with the order of bits reversed.
 */
extern const byte byte_reverse_bits[256];

/*
 * byte_right_mask[N] = a byte with N trailing 1s, 0 <= N <= 8.
 */
extern const byte byte_right_mask[9];

/*
 * byte_count_bits[B] = the number of 1-bits in a byte with value B.
 */
extern const byte byte_count_bits[256];

/*
 * byte_bit_run_length_N[B], for 0 <= N <= 7, gives the length of the
 * run of 1-bits starting at bit N in a byte with value B,
 * numbering the bits in the byte as 01234567.  If the run includes
 * the low-order bit (i.e., might be continued into a following byte),
 * the run length is increased by 8.
 */
extern const byte
    byte_bit_run_length_0[256], byte_bit_run_length_1[256],
    byte_bit_run_length_2[256], byte_bit_run_length_3[256],
    byte_bit_run_length_4[256], byte_bit_run_length_5[256],
    byte_bit_run_length_6[256], byte_bit_run_length_7[256];

/*
 * byte_bit_run_length[N] points to byte_bit_run_length_N.
 * byte_bit_run_length_neg[N] = byte_bit_run_length[-N & 7].
 */
extern const byte *const byte_bit_run_length[8];
extern const byte *const byte_bit_run_length_neg[8];

/*
 * byte_acegbdfh_to_abcdefgh[acegbdfh] = abcdefgh, where the letters
 * denote the individual bits of the byte.
 */
extern const byte byte_acegbdfh_to_abcdefgh[256];

#endif /* gsbittab_INCLUDED */
