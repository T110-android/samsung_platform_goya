#!/bin/sh
# hack to restart using tclsh \
exec tclsh "$0" "$@"

#    Copyright (C) 2000 Aladdin Enterprises.  All rights reserved.
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


# $Id: errlist.tcl,v 1.5 2007/08/01 14:26:55 jemarch Exp $

# Usage:
#	errlist < compiler-output-log > interleaved-listing

set inname ""
while {[gets stdin line] >= 0} {
    if {![regexp {^([./0-9a-zA-Z_]+):([0-9]+):} $line skip fname lno]} {continue}
    if {$fname != $inname} {
	if {$inname != ""} {close $infile}
	set infile [open $fname]
	set inname $fname
	set inlno 1
    }
    puts $line
    while {$inlno < $lno} {
	gets $infile
	incr inlno
    }
    while {$inlno <= $lno} {
	if {[gets $infile inline] >= 0} {
	    puts $inline
	}
	incr inlno
    }
    puts ""
}
if {$inname != ""} {
    close $infile
}
