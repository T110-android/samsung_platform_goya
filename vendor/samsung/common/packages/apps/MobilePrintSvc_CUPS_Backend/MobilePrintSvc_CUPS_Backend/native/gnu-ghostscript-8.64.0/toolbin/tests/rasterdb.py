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

 
# $Id: rasterdb.py,v 1.8 2008/05/04 14:35:07 Arabidopsis Exp $
 
# rasterdb.py
#
# methods to put and retrieve files to and from the raster database

import os, gzip
from stat import *
import gsconf

def exists(file, dbdir=gsconf.rasterdbdir):
    x = 0
    filename=dbdir + file + '.gz'
    try:
        mode = os.stat(dbdir + file + '.gz')[ST_MODE]
        if S_ISREG(mode):
            x = 1
    except:
        pass
    
    return x

def get_file(file, dbdir=gsconf.rasterdbdir, output=None):
    if exists(file, dbdir):
        if output:
            ofile = output
        else:
            ofile = file
        zf = gzip.open(dbdir + file + '.gz')
        f = open(ofile, 'w')
        data = zf.read(1024)
        while data:
            f.write(data)
            data = zf.read(1024)
        zf.close()
        f.close()
    else:
        print "rasterdb.get_file: does not exist",file

def put_file(file, dbdir=gsconf.rasterdbdir):
    mode = os.stat(file)[ST_MODE]
    if S_ISREG(mode):
        f = open(file)
        zf = gzip.open(dbdir + file + '.gz', 'w')
        data = f.read(1024)
        while data:
            zf.write(data)
            data = f.read(1024)
        f.close()
        zf.close()
                       
