#!/usr/bin/env python

#    Copyright (C) 2001-2004 Artifex Software Inc.
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

# $Id: gscheck_testfiles.py,v 1.2 2008/05/04 14:35:05 Arabidopsis Exp $

#
# gscheck_raster.py
#
# rasterizes input files in several configurations and checks them
# against known baselines
#

myself="gscheck_testfiles.py"
import sys, os
import gsconf, gstestutils


def add_compare_test(suite, gsroot, testfile, device, dpi, band, track, now=None):

    logdir=gsconf.logdir
    if now == None:
        now=time.strftime("%Y-%m-%d-%H:%M:%S", time.localtime())
    prefix=logdir+now+"."

    log_stdout=prefix+gsconf.gs_stdout
    log_stderr=prefix+gsconf.gs_stderr

    suite.addTest(GSCompareTestCase(gsroot=gsroot,
                                    file=gsconf.comparefiledir + testfile,
                                    device=device,dpi=dpi,band=band,
                                    log_stdout=log_stdout,
                                    log_stderr=log_stderr,
                                    track_daily=track,now=now)
                  )

def addTests(suite,gsroot,now,options=None, **args):
    if args.has_key('track'):
        track = args['track']
    else:
        track = 0

    # get a list of test files
    comparefiles = os.listdir(gsconf.comparefiledir)
    comparefiles.sort()

    if sys.modules["gsconf"].__dict__.has_key("revision"):
        print myself,gsconf.revision
    
#    for testfile in comparefiles:
#        print myself,testfile

    for testfile in comparefiles:
        if gsutil.check_extension(testfile):
            print testfile

if __name__ == '__main__':
    gstestutils.gsRunTestsMain(addTests)
