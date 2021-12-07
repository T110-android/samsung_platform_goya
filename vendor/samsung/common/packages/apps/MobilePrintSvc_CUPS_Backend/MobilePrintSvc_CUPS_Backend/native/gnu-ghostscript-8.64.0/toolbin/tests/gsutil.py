#    Copyright (C) 2004 Artifex Software Inc.
#    All Rights Reserved.
# 
# This file is part of GNU ghostscript
#
# GNU ghostscript is free software; you can redistribute it and/or
# modify it under the terms of the version 2 of the GNU General Public
# License as published by the Free Software Foundation.
#
# This software is provided AS-IS with no warranty, either express or
# implied. That is, this program is distributed in the hope that it will 
# be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA, 02110-1301.
# 

# $Id: gsutil.py,v 1.3 2008/05/04 14:35:05 Arabidopsis Exp $

# gsutil.py
#
# this module contains utility routines used by the regression test scripts

import string

def check_extension(fn):
    f = string.lower(fn)
    if f[-3:] == '.ps' or f[-4:] == '.pdf' or f[-4:] == '.eps' \
       or f[-3:] == '.ai':
        return 1
    return 0
