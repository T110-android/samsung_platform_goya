#!/usr/bin/env python
# -*- Mode: python -*-

#    Copyright (C) 2007 Artifex Software Inc.
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


import optparse
import sys, os
import types

def parseCommandLineBasic(optionsParser=None,args=sys.argv):
    optionsParser.add_option('--verbose','-v',action='store_true',help="noisy execution")

    myself=os.path.basename(args[0])
    options,arguments=optionsParser.parse_args()
    options.myself=myself
    return (options,arguments)

def parseCommandLine(optionsParser=None,args=sys.argv,revisionSkip=False,testfileSkip=False,listfileSkip=False,deviceSkip=False):
    if not optionsParser:
        optionsParser=optparse.OptionParser()

    if not testfileSkip:
        optionsParser.add_option('--testfile','-t',action='store',help="testfile:\"test\"",default="test")

    if not listfileSkip:
        optionsParser.add_option('--listfile','-l',action='store',help="listfile:\"list\"",default="list")

    if not revisionSkip:
        optionsParser.add_option('--revision','-e',action='store',help="revision:HEAD",default="HEAD")

    if not deviceSkip:
        optionsParser.add_option('--device','-d',action='store',help="output device:ppmraw",default="ppmraw")
        optionsParser.add_option('--resolution','-r',action='store',help="output resolution:300",default="300")
        optionsParser.add_option('--banding','-b',action='store',help="output banding:False",default=False)

    optionsParser.add_option('--quiet','-q',action='store_true',help="quiet execution")
    optionsParser.add_option('--verbose','-v',action='store_true',help="noisy execution")

    optionsParser.add_option('--nocleanup','-k',action='store_true',help="do not delete intermediate files")

    myself=os.path.basename(args[0])
    options,arguments=optionsParser.parse_args()
    options.myself=myself
    return (options,arguments)

if __name__ == "__main__":

    optionsParser=optparse.OptionParser()
    optionsParser.add_option('--option','-o',action='store_true',help="sample additional option")
    optionsParser.add_option('--nosvn','-s',action='store_true',help="no not update from svn")
    optionsParser.add_option('--nomake','-m',action='store_true',help="no not make")

    (options,arguments)=myoptparse.parseCommandLine(optionsParser)
    print options.revision
    print options.testfile
    print arguments

#   (options,arguments)=parseCommandLine(optionsParser,revisionSkip=True)
#   (options,arguments)=parseCommandLine(optionsParser,revisionSkip=True,testfileSkip=True,listfileSkip=True,deviceSkip=True):
