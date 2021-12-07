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

/* $Id: stdpn.h,v 1.6 2007/09/11 15:23:57 Arabidopsis Exp $ */
/* Pn macros for pre-ANSI compiler compatibility */

#ifndef stdpn_INCLUDED
#  define stdpn_INCLUDED

/*
 * We formerly supported "traditional" (pre-ANSI) C compilers, by using
 * these macros for formal parameter lists and defining them as empty
 * for pre-ANSI compilers, with the syntax
 *      resulttype func(Pn(arg1, ..., argn));
 * However, we no longer support pre-ANSI compilers; these macros are
 * deprecated (should not be used in new code), and eventually will be
 * removed.
 */

#define P0() void
#define P1(t1) t1
#define P2(t1,t2) t1,t2
#define P3(t1,t2,t3) t1,t2,t3
#define P4(t1,t2,t3,t4) t1,t2,t3,t4
#define P5(t1,t2,t3,t4,t5) t1,t2,t3,t4,t5
#define P6(t1,t2,t3,t4,t5,t6) t1,t2,t3,t4,t5,t6
#define P7(t1,t2,t3,t4,t5,t6,t7) t1,t2,t3,t4,t5,t6,t7
#define P8(t1,t2,t3,t4,t5,t6,t7,t8) t1,t2,t3,t4,t5,t6,t7,t8
#define P9(t1,t2,t3,t4,t5,t6,t7,t8,t9) t1,t2,t3,t4,t5,t6,t7,t8,t9
#define P10(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10) t1,t2,t3,t4,t5,t6,t7,t8,t9,t10
#define P11(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11) t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11
#define P12(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12) t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12
#define P13(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13) t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13
#define P14(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14) t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14
#define P15(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15) t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15
#define P16(t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16) t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16

#endif /* stdpn_INCLUDED */
