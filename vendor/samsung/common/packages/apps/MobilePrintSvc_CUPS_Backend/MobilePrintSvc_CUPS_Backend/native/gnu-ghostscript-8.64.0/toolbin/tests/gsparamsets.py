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


# $Id: gsparamsets.py,v 1.7 2008/05/04 14:35:06 Arabidopsis Exp $

# gsparamsets.py
#
# Parameter sets for regression testing with gs

class _GSParamSet:
    def __init__(self, device, resolution, banding):
        self.device = device
        self.resolution = resolution
        self.banding = banding
 
testparamsets_name = ""

testparamsets_maximum = [
    # mono
    _GSParamSet('pbmraw', 72, 0),
    _GSParamSet('pbmraw', 72, 1),
    _GSParamSet('pbmraw', 300, 0),
    _GSParamSet('pbmraw', 300, 1),
#   _GSParamSet('pbmraw', 600, 0),
#   _GSParamSet('pbmraw', 600, 1),

    # grayscale
    _GSParamSet('pgmraw', 72, 0),
    _GSParamSet('pgmraw', 72, 1),
    _GSParamSet('pgmraw', 300, 0),
    _GSParamSet('pgmraw', 300, 1),
#   _GSParamSet('pgmraw', 600, 0),
#   _GSParamSet('pgmraw', 600, 1),

    # color
    _GSParamSet('ppmraw', 72, 0),
    _GSParamSet('ppmraw', 72, 1),
    _GSParamSet('ppmraw', 300, 0),
    _GSParamSet('ppmraw', 300, 1),
#   _GSParamSet('ppmraw', 600, 0),
#   _GSParamSet('ppmraw', 600, 1),

    # 1-bit CMYK
    _GSParamSet('pkmraw', 72, 0),
    _GSParamSet('pkmraw', 72, 1),
    _GSParamSet('pkmraw', 300, 0),
    _GSParamSet('pkmraw', 300, 1),
#   _GSParamSet('pkmraw', 600, 0),
#   _GSParamSet('pkmraw', 600, 1)
]

testparamsets_full = [
    # mono
    _GSParamSet('pbmraw', 72, 0),
    _GSParamSet('pbmraw', 300, 0),
    _GSParamSet('pbmraw', 300, 1),

    # grayscale
    _GSParamSet('pgmraw', 72, 0),
    _GSParamSet('pgmraw', 300, 0),
    _GSParamSet('pgmraw', 300, 1),

    # color
    _GSParamSet('ppmraw', 72, 0),
    _GSParamSet('ppmraw', 300, 0),
    _GSParamSet('ppmraw', 300, 1),

    # 1-bit CMYK
    _GSParamSet('pkmraw', 72, 0),
    _GSParamSet('pkmraw', 300, 0),
    _GSParamSet('pkmraw', 300, 1),
]
testparamsets_minimum = [
    # mono
    _GSParamSet('pbmraw', 72, 0),
    _GSParamSet('pbmraw', 300, 0),
    _GSParamSet('pgmraw', 300, 0),
    _GSParamSet('ppmraw', 300, 0),
]

testparamsets_one = [
    # mono
    _GSParamSet('pbmraw', 300, 0),
]

pdftestparamsets_full = [
    # color
    _GSParamSet('ppmraw', 72, 0),
    _GSParamSet('ppmraw', 300, 0),
    
    # 1-bit CMYK
    _GSParamSet('pkmraw', 300, 0)
]

pdftestparamsets_minimum = [
    # color
    _GSParamSet('ppmraw', 300, 0),
]

pdftestparamsets_maximum = pdftestparamsets_full

testparamsets = testparamsets_minimum
pdftestparamsets = pdftestparamsets_minimum
testparamsets_name = "minimum"

testparamsets = testparamsets_maximum
pdftestparamsets = pdftestparamsets_maximum
testparamsets_name = "maximum"

testparamsets = testparamsets_one
pdftestparamsets = pdftestparamsets_minimum
testparamsets_name = "one"

testparamsets = testparamsets_full
pdftestparamsets = pdftestparamsets_minimum
testparamsets_name = "full"

