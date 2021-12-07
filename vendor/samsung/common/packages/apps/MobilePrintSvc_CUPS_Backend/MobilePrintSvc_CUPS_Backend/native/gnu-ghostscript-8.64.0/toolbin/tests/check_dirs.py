#!/usr/bin/env python

#    Copyright (C) 2002 Artifex Software, Inc.
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


# Check the contents of Ghostscript directories.

import os
from gstestutils import GSTestCase, gsRunTestsMain

################ Check that there are no files in the top-level directory,
################ other than a specified list.

class GSCheckEmptyTopDirectory(GSTestCase):

    def __init__(self, root, allowedFiles = []):
        self.root = root
        self.allowedFiles = allowedFiles
        GSTestCase.__init__(self)

    def runTest(self):
        """The top-level directory must not have extraneous files."""
        import glob, os.path
        messages = []
        for f in glob.glob(os.path.join(self.root, '*')):
            if not (os.path.isdir(f) or os.path.islink(f) or os.path.basename(f) in self.allowedFiles):
                messages.append(f)
	messages.sort()
        self.failIfMessages(messages)

################ Main program

gsFilesInTopDirectory = ['LICENSE', 'autogen.sh', 'Makefile',
			 'configure', 'config.log', 'config.status']
# Add the tests defined in this file to a suite.

def addTests(suite, gsroot, **args):
    suite.addTest(GSCheckEmptyTopDirectory(gsroot, gsFilesInTopDirectory))

if __name__ == "__main__":
    gsRunTestsMain(addTests)
