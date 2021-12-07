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

/* $Id: ttfinp.c,v 1.6 2007/09/11 15:23:48 Arabidopsis Exp $ */
/* A TT font input support. */


#include "ttmisc.h"

#include "ttfoutl.h"
#include "ttfsfnt.h"
#include "ttfinp.h"

unsigned char ttfReader__Byte(ttfReader *r)
{   unsigned char b; 

    r->Read(r, &b, 1); 
    return b;
}

signed char ttfReader__SignedByte(ttfReader *r)
{   signed char b; 

    r->Read(r, &b, 1); 
    return b;
}

signed short ttfReader__Short(ttfReader *r)
{   unsigned char buf[2]; 

    r->Read(r, buf, 2);
    return ((int16)buf[0] << 8) | (int16)buf[1];
}

unsigned short ttfReader__UShort(ttfReader *r)
{   unsigned char buf[2]; 

    r->Read(r, buf, 2);
    return ((uint16)buf[0] << 8) | (uint16)buf[1];
}

unsigned int ttfReader__UInt(ttfReader *r)
{   unsigned char buf[4]; 

    r->Read(r, buf, 4);
    return ((int32)buf[0] << 24) | ((int32)buf[1] << 16) | 
	   ((int32)buf[2] <<  8) |  (int32)buf[3];
}

signed int ttfReader__Int(ttfReader *r)
{   
    return (int)ttfReader__UInt(r);
}

