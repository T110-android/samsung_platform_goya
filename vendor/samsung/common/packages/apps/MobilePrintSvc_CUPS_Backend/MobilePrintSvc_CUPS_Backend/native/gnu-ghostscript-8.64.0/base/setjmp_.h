/* Copyright (C) 2006 Artifex Software, Inc.
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

/* $Id: setjmp_.h,v 1.4 2007/09/11 15:23:43 Arabidopsis Exp $ */
/* Macros for making sure setjmp buffers are correctly aligned. */

#ifndef setjmp_INCLUDED
#  define setjmp_INCLUDED

#include <setjmp.h>

typedef struct {
    char c;
    jmp_buf j;
} gsfix_jmp_buf_test;

#define gsfix_jmp_buf_align ((size_t)&((gsfix_jmp_buf_test*)0)->j)

typedef struct {
    unsigned char stuff[sizeof(jmp_buf) + gsfix_jmp_buf_align];
} gsfix_jmp_buf;


/* This could be moved into a function, but it's fairly harmless as a macro. */
#define find_jmp_buf(gsfjb) 				\
  (*(jmp_buf *)(					\
   ((size_t)(gsfjb).stuff + gsfix_jmp_buf_align) 	\
   & ~(size_t)(gsfix_jmp_buf_align-1)			\
  ))

#endif /* setjmp_INCLUDED */

