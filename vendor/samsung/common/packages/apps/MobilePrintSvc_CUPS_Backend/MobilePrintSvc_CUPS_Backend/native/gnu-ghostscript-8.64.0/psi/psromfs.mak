#  Copyright (C) 2001-2006 Artifex Software, Inc.
#  All Rights Reserved.
#
#  This file is part of GNU ghostscript
#
#  GNU ghostscript is free software; you can redistribute it and/or modify it under
#  the terms of the version 2 of the GNU General Public License as published by the Free Software
#  Foundation.
#
#  GNU ghostscript is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
#  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along with
#  ghostscript; see the file COPYING. If not, write to the Free Software Foundation,
#  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# $Id: psromfs.mak,v 1.1 2008/05/04 14:34:47 Arabidopsis Exp $
# mkromfs macros for PostScript %rom% when COMPILE_INITS=1

# The following list of files needed by the interpreter is maintained here.
# This changes infrequently, but is a potential point of bitrot, but since
# unix-inst.mak uses this macro, problems should surface when testing installed
# versions.

#	Resource files go into Resource/...
#	The init files are in the %rom%Resource/Init/ directory
#       Any EXTRA_INIT_FILES go into %rom%lib/

RESOURCE_LIST=CIDFont$(D) CMap$(D) ColorSpace$(D) Decoding$(D) Encoding$(D) Font$(D) IdiomSet$(D) Init$(D) ProcSet$(D) SubstCID$(D)

#	Note: gs_cet.ps is only needed to match Adobe CPSI defaults
PS_ROMFS_ARGS=-c -P $(PSRESDIR)$(D) -d Resource/ $(RESOURCE_LIST) \
   -d lib/ -P $(PSLIBDIR)$(D) $(EXTRA_INIT_FILES)

# We'd really like to have all of the files in Resourc/Init/ be dependencies
# for COMPILE_INITS=1, but we settle for just the main one. "touch" it if
# you change any Resource/Init files to force re-build of the %rom% data.
PS_ROMFS_DEPS=$(PSRESDIR)$(D)Init$(D)$(GS_INIT)
