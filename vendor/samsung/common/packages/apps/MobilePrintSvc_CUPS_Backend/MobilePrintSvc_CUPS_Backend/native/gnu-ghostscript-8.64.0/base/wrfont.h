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

/* $Id: wrfont.h,v 1.6 2007/09/11 15:24:34 Arabidopsis Exp $ */
/*
Header for support functions to serialize fonts as PostScript code that can
then be passed to FreeType via the FAPI FreeType bridge.
Started by Graham Asher, 9th August 2002.
*/

#ifndef wrfont_INCLUDED
#define wrfont_INCLUDED

#include "stdpre.h"

typedef struct WRF_output_
	{
	unsigned char* m_pos;
	long m_limit;
	long m_count;
	bool m_encrypt;
	unsigned short m_key;
	} WRF_output;

void WRF_init(WRF_output* a_output,unsigned char* a_buffer,long a_buffer_size);
void WRF_wbyte(WRF_output* a_output,unsigned char a_byte);
void WRF_wtext(WRF_output* a_output,const unsigned char* a_string,long a_length);
void WRF_wstring(WRF_output* a_output,const char* a_string);
void WRF_wfloat(WRF_output* a_output,double a_float);
void WRF_wint(WRF_output* a_output,long a_int);

#endif
