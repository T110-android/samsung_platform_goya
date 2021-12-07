#  Copyright (C) 2001-2006 Artifex Software, Inc.
#  All Rights Reserved.
#
#  This file is part of GNU ghostscript
#
#  GNU ghostscript is free software; you can redistribute it and/or
#  modify it under the terms of the version 2 of the GNU General
#  Public License as published by the Free Software Foundation.
#
#  GNU ghostscript is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
#  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along with
#  ghostscript; see the file COPYING. If not, write to the Free Software Foundation,
#  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# $Id: icclib.mak,v 1.7 2007/09/11 15:23:57 Arabidopsis Exp $
# makefile for icclib library code.
# Users of this makefile must define the following:
#	GLSRCDIR - the graphic library source directory
#	ICCSRCDIR - the icclib source directory
#	ICCGENDIR - the generated intermediate file directory
#	ICCOBJDIR - the object directory

# This partial makefile compiles Graeme W. Gill's icclibfor use in Ghostscript.
#
# The original source for the code in this directory may be accessed via
#   http://web.access.net.au/argyll/color.html
# For information on ICC color profiles in general, see the International
# Color Consortium's web site at
#   http://www.color.org
#
# This makefile has been tested with version 2.0 of the icclib code. If you
# are working with a later version, you may need to update the ICC profile
# version number macro ICCPROFVER.

ICCPROFVER=9809

ICCSRC=$(ICCSRCDIR)$(D)
ICCGEN=$(ICCGENDIR)$(D)
ICCOBJ=$(ICCOBJDIR)$(D)
ICCO_=$(O_)$(ICCOBJ)

# We need D_, _D_, and _D because the OpenVMS compiler uses different
# syntax from other compilers.
# ICCI_ and ICCF_ are defined in gs.mak.
ICC_INCL=$(I_)$(ICCI_) $(II)$(GLSRCDIR) $(II)$(GLGENDIR)$(_I)
ICC_CCFLAGS=$(ICC_INCL) $(ICCF_) 
ICC_CC=$(CC_) $(ICC_CCFLAGS)

# Define the name of this makefile.
ICCLIB_MAK=$(GLSRC)icclib.mak

icc.clean : icc.config-clean icc.clean-not-config-clean

### WRONG.  MUST DELETE OBJ AND GEN FILES SELECTIVELY.
icc.clean-not-config-clean :
#	echo $(ICCSRC) $(ICCGEN) $(ICCOBJ) $(ICCO_)
	$(EXP)$(ECHOGS_XE) $(ICCSRC) $(ICCGEN) $(ICCOBJ) $(ICCO_)
	$(RM_) $(ICCOBJ)*.$(OBJ)

icc.config-clean :
	$(RMN_) $(ICCGEN)icclib*.dev

ICCDEP=$(AK)

# Code common to compression and decompression.

icclib_=$(ICCOBJ)icc.$(OBJ)
$(ICCGEN)icclib.dev : $(ICCLIB_MAK) $(ECHOGS_XE) $(icclib_)
	$(SETMOD) $(ICCGEN)icclib $(icclib_)

icc_h=$(ICCSRC)$(D)icc.h $(ICCSRC)$(D)icc$(ICCPROFVER).h

$(ICCOBJ)icc.$(OBJ) : $(ICCSRC)icc.c $(ICCDEP) $(ECHOGS_XE) $(icc_h)
#	echo $(ICC_CCFLAGS)
	$(EXP)$(ECHOGS_XE) $(ICC_CCFLAGS)
	$(ICC_CC) $(ICCO_)icc.$(OBJ) $(C_) $(ICCSRC)icc.c
