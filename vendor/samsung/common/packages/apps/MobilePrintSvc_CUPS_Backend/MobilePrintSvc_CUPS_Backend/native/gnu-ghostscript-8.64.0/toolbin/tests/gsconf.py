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


# $Id: gsconf.py,v 1.8 2008/05/04 14:35:06 Arabidopsis Exp $

# gsconf.py
#
# configuration file parser for regression tests

import os
import re
import sys
import time

configdir = os.path.dirname(sys.argv[0])
if len(configdir) > 0:
    configdir = configdir + "/"

def parse_config(file=configdir+"testing.cfg"):
    try:
        cf = open(file, "r")
    except:
        print "ERROR: Could not open config file '%s'." % (file,)
        return

    config_re = re.compile("^([^\s]+)\s+(.*)$")

    for l in cf.readlines():
        # strip off EOL chars
        while l and (l[-1] == '\r' or l[-1] == '\n'):
            l = l[:-1]

        # ignore comments and blank lines
        if not l or l[0] == '#':
            continue

        m = config_re.match(l)
        if m:
            sys.modules["gsconf"].__dict__[m.group(1)] = m.group(2)


def get_dailydb_name():
    return dailydir + time.strftime("%Y%m%d", time.localtime()) # mhw + ".db"

parse_config()
