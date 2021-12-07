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

/* $Id: write_t2.h,v 1.6 2007/09/11 15:23:55 Arabidopsis Exp $ */
/*
Header for functions to serialize a type 2 (CFF) font so that it can
then be passed to FreeType via the FAPI FreeType bridge.
Started by Graham Asher, 9th August 2002.
*/

#ifndef write_t2_INCLUDED
#define write_t2_INCLUDED

#include "ifapi.h"

long FF_serialize_type2_font(FAPI_font* a_fapi_font,unsigned char* a_buffer,long a_buffer_size);

#endif
