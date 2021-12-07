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

/* $Id: iostack.h,v 1.7 2007/09/11 15:24:41 Arabidopsis Exp $ */
/* Generic operand stack API */

#ifndef iostack_INCLUDED
#  define iostack_INCLUDED

#include "iosdata.h"
#include "istack.h"

/* Define pointers into the operand stack. */
typedef s_ptr os_ptr;
typedef const_s_ptr const_os_ptr;

#endif /* iostack_INCLUDED */
