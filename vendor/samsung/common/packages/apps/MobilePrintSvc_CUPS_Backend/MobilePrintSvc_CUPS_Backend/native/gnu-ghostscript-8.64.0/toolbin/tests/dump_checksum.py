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

# $Id: dump_checksum.py,v 1.2 2008/05/04 14:35:05 Arabidopsis Exp $

#
# dump_baseline.py [<dbfile>]
#
# dumps (prints out) the contents of the baselinedb

import string, sys, anydbm, gsconf, os, optparse, myoptparse

if __name__ == "__main__":

    optionsParser=optparse.OptionParser()
    (options,arguments)=myoptparse.parseCommandLineBasic(optionsParser)

    if len(arguments) == 1:
        filename=arguments.pop(0)
    else:
        filename=gsconf.baselinedb

    if not os.path.exists(filename):
        print "cannot open",filename
        sys.exit(1)

    print "opening ", filename
    db = anydbm.open(filename)

    if db:
        keys=db.keys()
        keys.sort()

        count=0
        for k in keys:
            count+=1
            if options.verbose:
                print '%s %s' % (db[k], k)

        print options.myself,"number of entries",count,"in database",filename

    else:
        print options.myself,"no entries in database",filename
