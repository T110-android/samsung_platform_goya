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

# $Id: make_baselinedb.py,v 1.2 2008/05/04 14:35:06 Arabidopsis Exp $

#
# make_testdb <dir>
#
# this script creates the gstestdb database and intializes it
# with the md5sums of the test data files run through the
# baseline ghostscript

import gstestgs
import gsconf
import gssum
import gsparamsets
import rasterdb
import anydbm
import os, sys
import string
import gsutil

def make_compare_entry(ifile, device, dpi, band):
    ofile = "%s.%s.%d.%d" % (ifile, device, dpi, band)
    print "creating entry: " + ofile + "...",
    sys.stdout.flush()
    
    gs = gstestgs.Ghostscript()
    gs.log_stdout = gsconf.log_stdout
    gs.log_stderr = gsconf.log_stderr
    gs.command = gsconf.baselinegs
    gs.infile = gsconf.comparefiledir + ifile
    gs.outfile = ofile
    gs.device = device
    gs.dpi = dpi
    gs.band = band

    if not gssum.exists(ofile):
        if gs.process():
	    try:
       	        gssum.add_file(ofile)
                rasterdb.put_file(ofile)
    	        os.unlink(ofile)
    	        print "done."
            except OSError:
                print "no output produced."
        else:
	    print "error."
    else:
	print "exists."

def make_pdfcompare_entry(ifile, device, dpi, band):
    ofile = "%s.pdf.%s.%d.%d" % (ifile, device, dpi, band)
    print "creating entry: " + ofile + "...",
    sys.stdout.flush()

    if gssum.exists(ofile):
        print "exists."
        return
    
    gs = gstestgs.Ghostscript()
    gs.log_stdout = gsconf.log_stdout
    gs.log_stderr = gsconf.log_stderr
    gs.command = gsconf.baselinegs
    gs.infile = gsconf.comparefiledir + ifile
    gs.dpi = dpi
    gs.band = band

    # make file->PDF

    tfile = ofile + ".pdf"
    gs.outfile = tfile
    gs.device = 'pdfwrite'
    gs.dpi = None

    if not gs.process():
        print "error."
        return

    gs.infile = tfile
    gs.outfile = ofile
    gs.device = device
    gs.dpi = dpi

    if gs.process():
        try:
            gssum.add_file(ofile)
            rasterdb.put_file(ofile)
            os.unlink(tfile)
            os.unlink(ofile)
            print "done."
        except OSError:
            print "no output produced."
    else:
        print "error."


if __name__ == "__main__":
    print "this script is only useful when starting a new database"
    print "exit now"
    sys.exit(1)

    # create the baselinedb
    db = anydbm.open(gsconf.baselinedb, 'c')
    db.close()

    files = os.listdir(gsconf.comparefiledir)
    for f in files:
        if gsutil.check_extension(f):
            for params in gsparamsets.testparamsets:
                make_compare_entry(f, params.device, params.resolution, params.banding)
            for params in gsparamsets.pdftestparamsets:
                make_pdfcompare_entry(f, params.device, params.resolution, params.banding)
