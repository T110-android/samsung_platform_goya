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
# $Id: unix-end.mak,v 1.10 2008/05/04 14:34:57 Arabidopsis Exp $
# Partial makefile common to all Unix and Desqview/X configurations.
# This is the next-to-last part of the makefile for these configurations.

# Define the rule for building standard configurations.
STDDIRS:
	@if test ! -d $(BINDIR); then mkdir $(BINDIR); fi
	@if test ! -d $(GLGENDIR); then mkdir $(GLGENDIR); fi
	@if test ! -d $(GLOBJDIR); then mkdir $(GLOBJDIR); fi
	@if test ! -d $(PSGENDIR); then mkdir $(PSGENDIR); fi
	@if test ! -d $(PSOBJDIR); then mkdir $(PSOBJDIR); fi

# Define a rule for building profiling configurations.
PGDIRS: STDDIRS
	@if test ! -d $(BINDIR)/$(PGRELDIR); then mkdir $(BINDIR)/$(PGRELDIR); fi
	@if test ! -d $(GLGENDIR)/$(PGRELDIR); then mkdir $(GLGENDIR)/$(PGRELDIR); fi
	@if test ! -d $(GLOBJDIR)/$(PGRELDIR); then mkdir $(GLOBJDIR)/$(PGRELDIR); fi
	@if test ! -d $(PSGENDIR)/$(PGRELDIR); then mkdir $(PSGENDIR)/$(PGRELDIR); fi
	@if test ! -d $(PSOBJDIR)/$(PGRELDIR); then mkdir $(PSOBJDIR)/$(PGRELDIR); fi

PGDEFS=GENOPT='-DPROFILE' CFLAGS='$(CFLAGS_PROFILE) $(GCFLAGS) $(XCFLAGS)'\
 LDFLAGS='$(XLDFLAGS) -pg' XLIBS='Xt SM ICE Xext X11'\
 BINDIR=$(BINDIR)/$(PGRELDIR)\
 GLGENDIR=$(GLGENDIR)/$(PGRELDIR) GLOBJDIR=$(GLOBJDIR)/$(PGRELDIR)\
 PSGENDIR=$(PSGENDIR)/$(PGRELDIR) PSOBJDIR=$(PSOBJDIR)/$(PGRELDIR)

pg: PGDIRS
	$(MAKE) $(PGDEFS) default

pgclean: PGDIRS
	$(MAKE) $(PGDEFS) clean

# Define a rule for building debugging configurations.
DEBUGDIRS: STDDIRS
	@if test ! -d $(BINDIR)/$(DEBUGRELDIR); then mkdir $(BINDIR)/$(DEBUGRELDIR); fi
	@if test ! -d $(GLGENDIR)/$(DEBUGRELDIR); then mkdir $(GLGENDIR)/$(DEBUGRELDIR); fi
	@if test ! -d $(GLOBJDIR)/$(DEBUGRELDIR); then mkdir $(GLOBJDIR)/$(DEBUGRELDIR); fi
	@if test ! -d $(PSGENDIR)/$(DEBUGRELDIR); then mkdir $(PSGENDIR)/$(DEBUGRELDIR); fi
	@if test ! -d $(PSOBJDIR)/$(DEBUGRELDIR); then mkdir $(PSOBJDIR)/$(DEBUGRELDIR); fi

DEBUGDEFS=GENOPT='-DDEBUG' CFLAGS='$(CFLAGS_DEBUG) $(GCFLAGS) $(XCFLAGS)'\
 BINDIR=$(BINDIR)/$(DEBUGRELDIR)\
 GLGENDIR=$(GLGENDIR)/$(DEBUGRELDIR) GLOBJDIR=$(GLOBJDIR)/$(DEBUGRELDIR)\
 PSGENDIR=$(PSGENDIR)/$(DEBUGRELDIR) PSOBJDIR=$(PSOBJDIR)/$(DEBUGRELDIR)

debug: DEBUGDIRS
	$(MAKE) $(DEBUGDEFS) default

debugclean: DEBUGDIRS
	$(MAKE) $(DEBUGDEFS) clean

# Emacs tags maintenance.

TAGS:
	etags -t $(GLSRC)*.[ch] $(PSSRC)*.[ch]
