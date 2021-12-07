#!/usr/bin/env python
# -*- Mode: python -*-

#    Copyright (C) 2001 Artifex Software Inc.
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

# $Id: dump_checksum_raw.py,v 1.2 2008/05/04 14:35:07 Arabidopsis Exp $

#
# dump_baseline.py [<dbfile>]
#
# dumps (prints out) the contents of the baselinedb

import string, sys, anydbm, gsconf, os

def compare_field_2(s1, s2):
    if string.split(s1,' ')[1] < string.split(s2,' ')[1]:
    	return -1
    else:
    	return 1

if len(sys.argv) == 2:
    name=sys.argv[1]
else:
    name=gsconf.baselinedb

if not os.path.exists(name):
    print "cannot open",name
    sys.exit(1)

print "opening ", name
db = anydbm.open(name)

# collect the database as strings
dump = []
for k in db.keys():
    print '-%50s- %s' % (k,db[k])

