#  Copyright (C) 2001-2008 Artifex Software, Inc.
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
# $Id: pcwin.mak,v 1.7 2007/09/11 15:24:34 Arabidopsis Exp $
# makefile for PC window system (MS Windows and OS/2) -specific device
# drivers.

# Define the name of this makefile.
PCWIN_MAK=$(GLSRC)pcwin.mak

# We have to isolate these in their own file because the MS Windows code
# requires special compilation switches, different from all other files
# and platforms.

### -------------------- The MS-Windows 3.n DLL ------------------------- ###

gp_mswin_h=$(GLSRC)gp_mswin.h
gsdll_h=$(GLSRC)gsdll.h
gsdllwin_h=$(GLSRC)gsdllwin.h

gdevmswn_h=$(GLSRC)gdevmswn.h $(GDEVH)\
 $(dos__h) $(memory__h) $(string__h) $(windows__h)\
 $(gp_mswin_h)

# This is deprecated and requires the interpreter / PSSRCDIR.
$(GLOBJ)gdevmswn.$(OBJ): $(GLSRC)gdevmswn.c $(gdevmswn_h) $(gp_h) $(gpcheck_h)\
 $(gsdll_h) $(gsdllwin_h) $(gsparam_h) $(gdevpccm_h)
	$(GLCCWIN) -I$(PSSRCDIR) $(GLO_)gdevmswn.$(OBJ) $(C_) $(GLSRC)gdevmswn.c

$(GLOBJ)gdevmsxf.$(OBJ): $(GLSRC)gdevmsxf.c $(ctype__h) $(math__h) $(memory__h) $(string__h)\
 $(gdevmswn_h) $(gsstruct_h) $(gsutil_h) $(gxxfont_h)
	$(GLCCWIN) $(GLO_)gdevmsxf.$(OBJ) $(C_) $(GLSRC)gdevmsxf.c

# An implementation using a DIB filled by an image device.
# This is deprecated and requires the interpreter / PSSRCDIR.
$(GLOBJ)gdevwdib.$(OBJ): $(GLSRC)gdevwdib.c\
 $(gdevmswn_h) $(gsdll_h) $(gsdllwin_h) $(gxdevmem_h)
	$(GLCCWIN) -I$(PSSRCDIR) $(GLO_)gdevwdib.$(OBJ) $(C_) $(GLSRC)gdevwdib.c

mswindll1_=$(GLOBJ)gdevmswn.$(OBJ) $(GLOBJ)gdevmsxf.$(OBJ) $(GLOBJ)gdevwdib.$(OBJ)
mswindll2_=$(GLOBJ)gdevemap.$(OBJ) $(GLOBJ)gdevpccm.$(OBJ)
mswindll_=$(mswindll1_) $(mswindll2_)
$(GLGEN)mswindll.dev: $(mswindll_)
	$(SETDEV) $(GLGEN)mswindll $(mswindll1_)
	$(ADDMOD) $(GLGEN)mswindll $(mswindll2_)

### -------------------- The MS-Windows DDB 3.n printer ----------------- ###

mswinprn_=$(GLOBJ)gdevwprn.$(OBJ) $(GLOBJ)gdevmsxf.$(OBJ)
$(DD)mswinprn.dev: $(mswinprn_)
	$(SETDEV) $(DD)mswinprn $(mswinprn_)

$(GLOBJ)gdevwprn.$(OBJ): $(GLSRC)gdevwprn.c $(gdevmswn_h) $(gp_h)
	$(GLCCWIN) $(GLO_)gdevwprn.$(OBJ) $(C_) $(GLSRC)gdevwprn.c

### -------------------- The MS-Windows DIB 3.n printer ----------------- ###

mswinpr2_=$(GLOBJ)gdevwpr2.$(OBJ)
$(DD)mswinpr2.dev: $(mswinpr2_) $(GLD)page.dev
	$(SETPDEV) $(DD)mswinpr2 $(mswinpr2_)

$(GLOBJ)gdevwpr2.$(OBJ): $(GLSRC)gdevwpr2.c $(PDEVH) $(windows__h)\
 $(gdevpccm_h) $(gp_h) $(gp_mswin_h)
	$(GLCCWIN) $(GLO_)gdevwpr2.$(OBJ) $(C_) $(GLSRC)gdevwpr2.c

### --------------------------- The OS/2 printer ------------------------ ###

os2prn_=$(GLOBJ)gdevos2p.$(OBJ)
$(DD)os2prn.dev: $(os2prn_) $(GLD)page.dev
	$(SETPDEV) $(DD)os2prn $(os2prn_)

$(GLOBJ)gdevos2p.$(OBJ): $(GLSRC)gdevos2p.c $(gp_h) $(gdevpccm_h) $(gdevprn_h) $(gscdefs_h)
	$(GLCC) $(GLO_)gdevos2p.$(OBJ) $(C_) $(GLSRC)gdevos2p.c
