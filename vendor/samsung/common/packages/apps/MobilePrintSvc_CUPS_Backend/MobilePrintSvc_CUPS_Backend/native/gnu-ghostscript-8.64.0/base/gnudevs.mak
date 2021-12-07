#    Copyright (C) 1999-2000 artofcode LLC. All rights reserved.
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
# 

# $Id: gnudevs.mak,v 1.3 2007/08/01 14:25:58 jemarch Exp $
# makefile for GPL'ed device drivers and features.

# All the code referenced in this file is licensed with the GNU General
# Public License.  It are user-contributed, and maintained (if at all) by
# users. If you have questions, please contact the author or contact
# person listed next to the entry for the device or feature.

# Define the name of this makefile.
GNUDEVS_MAK=$(GLSRC)gnudevs.mak

###### -------------------------- Features -------------------------- ######

# Author: Alexey Subbotin <A.Subbotin@lpi.ru>
# Feature: GNU readline
#
# The references from a gp_ module to the interpreter are a bug,
# but they are intrinsic to what this implementation does.
#
# In order to use GNU readline, you must add $(PSD)gnrdline to the
# definition of FEATURE_DEVS in your top-level makefile.
#
# If you want to use the GNU readline package with AIX, you must change
# the mention of termcap to curses in the line a few lines below here.

gnrdline_=$(PSOBJ)gp_gnrdl.$(OBJ)
$(PSD)gnrdline.dev : $(INT_MAK) $(ECHOGS_XE) $(gnrdline_)
	$(SETMOD) $(PSD)gnrdline $(gnrdline_)
	$(ADDMOD) $(PSD)gnrdline -lib readline termcap
	$(ADDMOD) $(PSD)gnrdline -replace $(GLD)strdline
	$(ADDMOD) $(PSD)gnrdline -ps gs_rdlin

$(PSOBJ)gp_gnrdl.$(OBJ) : $(PSSRC)gp_gnrdl.c $(AK)\
 $(ctype__h) $(malloc__h) $(memory__h) $(string__h)\
 $(gp_h) $(gscdefs_h) $(gsmalloc_h) $(gsmemory_h) $(gsstruct_h)\
 $(gxiodev_h) $(stream_h)\
 $(ghost_h) $(dstack_h) $(errors_h) $(ialloc_h) $(idict_h) $(iname_h) $(iutil_h)\
 $(ostack_h)
	$(PSCC) $(PSO_)gp_gnrdl.$(OBJ) $(C_) $(PSSRC)gp_gnrdl.c

###### --------------------------- Drivers -------------------------- ######

### ------------------------- DEC sixel displays ------------------------ ###

# Author: Phil Keegstra (keegstra@tonga.gsfc.nasa.gov)
# Device: DEC sixel displays
# This is a "printer" device, but it probably shouldn't be.
# I don't know why the implementor chose to do it this way.
sxlcrt_=$(GLOBJ)gdevln03.$(OBJ)
$(DD)sxlcrt.dev : $(sxlcrt_) $(DD)page.dev
	$(SETPDEV) $(DD)sxlcrt $(sxlcrt_)


### -------------------- Gimp-Print (stp) driver -------------------###

# To include this driver add '$(DD)stp.dev' to your DEVICE_DEVS
# line and define 'STPLIB=gimpprint' in your top-level makefile.
# See http://gimp-print.sourceforge.net/ for more information.

stp_=$(GLOBJ)gdevstp.$(OBJ)

$(DD)stp.dev : $(stp_) $(DD)page.dev
	$(SETPDEV) $(DD)stp $(stp_)
	$(ADDMOD) $(DD)stp -lib $(STPLIB)

$(GLOBJ)gdevstp.$(OBJ) : $(GLSRC)gdevstp.c \
 $(gdevprn_h) $(gdevpccm_h) $(gsparam_h)
	$(GLCC) $(GLO_)gdevstp.$(OBJ) $(C_) $(GLSRC)gdevstp.c


### -------------- cdj850 - HP 850c Driver under development ------------- ###

cdeskjet8_=$(GLOBJ)gdevcd8.$(OBJ) $(HPPCL)

# Author: Uli Wortmann (uliw@erdw.ethz.ch), Martin Gerbershagen (ger@ulm.temic.de)
# Printer: HP 670
$(DD)cdj670.dev : $(cdeskjet8_) $(DD)page.dev
	$(SETPDEV2) $(DD)cdj670 $(cdeskjet8_)

# Author: Uli Wortmann (uliw@erdw.ethz.ch)
# Printer: HP 850
$(DD)cdj850.dev : $(cdeskjet8_) $(DD)page.dev
	$(SETPDEV2) $(DD)cdj850 $(cdeskjet8_)

# Author: Uli Wortmann (uliw@erdw.ethz.ch), Martin Gerbershagen (ger@ulm.temic.de)
# Printer: HP 890
$(DD)cdj890.dev : $(cdeskjet8_) $(DD)page.dev
	$(SETPDEV2) $(DD)cdj890 $(cdeskjet8_)

# Author: Uli Wortmann (uliw@erdw.ethz.ch), Martin Gerbershagen (ger@ulm.temic.de)
# Printer: HP 1600
$(DD)cdj1600.dev : $(cdeskjet8_) $(DD)page.dev
	$(SETPDEV2) $(DD)cdj1600 $(cdeskjet8_)

$(GLOBJ)gdevcd8.$(OBJ) : $(GLSRC)gdevcd8.c $(PDEVH) $(math__h)\
 $(gsparam_h) $(gxlum_h) $(gdevpcl_h)
	$(GLCC) $(GLO_)gdevcd8.$(OBJ) $(C_) $(GLSRC)gdevcd8.c

### -------------- cdj880 - HP 880c Driver under development ------------- ###
### Since this driver is in the development phase it is not distributed    ###
### with Ghostscript, but is available from                                ###
###   http://www.proaxis.com/~mgelhaus/linux/software/hp880c/hp880c.html   ###

### NOTE:  Depends on the presence of the cdj850 section above.            ###

# Author: Matthew Gelhaus (mgelhaus@proaxis.com)
# Printer: HP 880c
$(DD)cdj880.dev : $(cdeskjet8_) $(DD)page.dev
	$(SETPDEV2) $(DD)cdj880 $(cdeskjet8_)

### ------------- cdj970 - HP 970Cxi Driver under development ------------- ###

cdeskjet9_=$(GLOBJ)gdevdj9.$(OBJ) $(HPPCL)

# Author: Rene Harsch (rene@harsch.net)
# Printer: HP 970Cxi
$(DD)cdj970.dev : $(cdeskjet9_) $(DD)page.dev
	$(SETPDEV2) $(DD)cdj970 $(cdeskjet9_)

$(GLOBJ)gdevdj9.$(OBJ) : $(GLSRC)gdevdj9.c $(PDEVH) $(math__h) $(string__h)\
 $(gsparam_h) $(gxlum_h) $(gdevpcl_h)
	$(GLCC) $(GLO_)gdevdj9.$(OBJ) $(C_) $(GLSRC)gdevdj9.c
 
### -------------------- The Brother HL-1250 printer -------------------- ###
### This printer is ljet4-compatible, plus support for source tray        ###
### selection and special 1200x600 dpi mode.                              ###

hl1250_=$(GLOBJ)gdevhl12.$(OBJ) $(HPDLJM)
$(DD)hl1250.dev : $(hl1250_) $(DD)page.dev
	$(SETPDEV) $(DD)hl1250 $(hl1250_)

$(DD)hl1240.dev : $(hl1250_) $(DD)page.dev
	$(SETPDEV) $(DD)hl1240 $(hl1250_)

# Author: Marek Michalkiewicz <marekm@linux.org.pl>
# Printer: Brother HL-1250 (may work with some other models too)
$(GLOBJ)gdevhl12.$(OBJ) : $(GLSRC)gdevhl12.c $(PDEVH) $(gdevdljm_h)
	$(GLCC) $(GLO_)gdevhl12.$(OBJ) $(C_) $(GLSRC)gdevhl12.c

### -------- The DEC LN03/DL2100/LA50/LA70/LA75 printer devices -------- ###

ln03_=$(GLOBJ)gdevln03.$(OBJ)

# Author: Ulrich Mueller (ulm@vsnhd1.cern.ch)
# Printer: DEC LN03
$(DD)ln03.dev : $(ln03_) $(DD)page.dev
	$(SETPDEV) $(DD)ln03 $(ln03_)

# Author: Nick Brown (nick.brown@coe.int)
# Printer: DEClaser 2100
$(DD)dl2100.dev : $(ln03_) $(DD)page.dev
	$(SETPDEV) $(DD)dl2100 $(ln03_)

# Author: Ian MacPhedran (macphed@dvinci.USask.CA)
# Printer: DEC LA50
$(DD)la50.dev : $(ln03_) $(DD)page.dev
	$(SETPDEV) $(DD)la50 $(ln03_)

# Author: Bruce Lowekamp (lowekamp@csugrad.cs.vt.edu)
# Printer: DEC LA70
$(DD)la70.dev : $(ln03_) $(DD)page.dev
	$(SETPDEV) $(DD)la70 $(ln03_)

# Author: Ian MacPhedran (macphed@dvinci.USask.CA)
# Printer: DEC LA75
$(DD)la75.dev : $(ln03_) $(DD)page.dev
	$(SETPDEV) $(DD)la75 $(ln03_)

# Author: Andre' Beck (Andre_Beck@IRS.Inf.TU-Dresden.de)
# Printer: DEC LA75plus
$(DD)la75plus.dev : $(ln03_) $(DD)page.dev
	$(SETPDEV) $(DD)la75plus $(ln03_)

$(GLOBJ)gdevln03.$(OBJ) : $(GLSRC)gdevln03.c $(PDEVH)
	$(GLCC) $(GLO_)gdevln03.$(OBJ) $(C_) $(GLSRC)gdevln03.c

### ----------------- The Okidata OkiPage 4w+ device ------------------- ###

oki4w_=$(GLOBJ)gdevop4w.$(OBJ)
$(DD)oki4w.dev : $(oki4w_) $(DD)page.dev
	$(SETPDEV) $(DD)oki4w $(oki4w_)

# Author: Ivan Schreter (ivan@shadow.sk)
# Printer: Okidata OkiPage 4w+
$(GLOBJ)gdevop4w.$(OBJ) : $(GLSRC)gdevop4w.c $(PDEVH)
	$(GLCC) $(GLO_)gdevop4w.$(OBJ) $(C_) $(GLSRC)gdevop4w.c

### ----------------- The Xerox XES printer device --------------------- ###

xes_=$(GLOBJ)gdevxes.$(OBJ)
$(DD)xes.dev : $(xes_) $(DD)page.dev
	$(SETPDEV) $(DD)xes $(xes_)

# Author: Peter Flass (flass@lbdrscs.bitnet)
# Printer: Xerox XES (2700, 3700, 4045, etc.)
$(GLOBJ)gdevxes.$(OBJ) : $(GLSRC)gdevxes.c $(PDEVH)
	$(GLCC) $(GLO_)gdevxes.$(OBJ) $(C_) $(GLSRC)gdevxes.c
