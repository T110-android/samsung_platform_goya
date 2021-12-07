#!/usr/bin/env python

#    Copyright (C) 2003 Artifex Software, Inc.
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


# $Id: check_comments.py,v 1.8 2008/05/04 14:35:06 Arabidopsis Exp $

# Check for C++ style comments

import os
import re
from glob import glob

import gsconf
from gstestutils import GSTestCase, gsRunTestsMain

class GSCheckForComments(GSTestCase):

    def __init__(self, root, dirName, extensions=['*'], skip=[]):
        self.root = root
        self.dirName = dirName
        self.extensions = extensions
        self.skip = skip
        GSTestCase.__init__(self)

    def shortDescription(self):
        return "All relevant files must not have C++ style comments"\
               " (checking %s)" % (self.dirName,)

    def runTest(self):
        d, extns, skip = self.root + self.dirName, self.extensions, self.skip
        skip = map((lambda o,d=d: d + os.sep + o), skip)
        incorrect = []
        for e in extns:
            for f in glob(d + os.sep + '*.' + e):
                if f in skip or os.path.isdir(f):
                    continue
                fp = open(f, 'r')
                text_code = fp.read()
                fp.close()

                pattern = re.compile("(\\\\.)|(\")|(')|(/\*)|(\*/)|(//)")
                mi = pattern.finditer(text_code)
                try:
                    inComment = 0
		    inString = 0
                    inStringSq = 0	# single quoted string
                    inStringDq = 0	# double quoted string
                    while 1:
                        m = mi.next()
                        mstr = m.group()
			if mstr[0] == '\\':		# skip quoted characters (may be ' or ")
			    continue
                        if mstr == '"' and not inComment and not inStringSq:
                            inStringDq = not inStringDq
			    inString = inStringDq
                            continue
                        if mstr == "'" and not inComment and not inStringDq:
                            inStringSq = not inStringSq
			    inString = inStringSq
                            continue
                        if not inString and mstr == '/*':
                            inComment = 1
                            continue
                        if inComment and mstr == '*/':
                            inComment = 0
                            continue
                        if not inString and not inComment and mstr == '//':
                            incorrect.append(f)
                            break
                except StopIteration:
                    continue
                    
        if incorrect:
            incorrect = ['These %d files have C++ style comments:' % (len(incorrect),)] + incorrect

        self.failIfMessages(incorrect)

## Main stuff

checkDirs = [
    ('src', ['c', 'h'],
     # list of exempt files
     ['dwdll.h',
      'dwimg.h',
      'dwinst.h',
      'dwsetup.h',
      'dwtext.h',
      'dwuninst.h',
      'gdevhpij.c',
      'dmmain.c',
      'gdevmac.c',
      'gdevmacxf.c',
      'gdevwdib.c',
      'gp_mac.c',
      'gp_macio.c',
      'macsysstat.h'
     ])
    ]

def addTests(suite, gsroot, **args):
    for dir, extns, skip in checkDirs:
        suite.addTest(GSCheckForComments(gsroot, dir, extns, skip))

if __name__ == "__main__":
    gsRunTestsMain(addTests)
