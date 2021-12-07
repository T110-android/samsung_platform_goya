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

# $Id: dump_checksum_plus.py,v 1.2 2008/05/04 14:35:05 Arabidopsis Exp $

#
# dump_baseline_plus.py [<dbfile> [name]]
#
# dumps (prints out) the contents of the baselinedb

import sys, anydbm, gsconf, os

args=sys.argv
myself=args.pop(0)

if len(args) > 0:
    name=args.pop(0)
else:
    print "no database name"
    sys.exit(1)

if not os.path.exists(name):
    print "cannot open",name
    sys.exit(1)
    
db = anydbm.open(name)

base=os.path.basename(name)

keys=db.keys()
keys.sort()
for k in keys:
    print "%50s %15s %s" % (k, base, db[k])

