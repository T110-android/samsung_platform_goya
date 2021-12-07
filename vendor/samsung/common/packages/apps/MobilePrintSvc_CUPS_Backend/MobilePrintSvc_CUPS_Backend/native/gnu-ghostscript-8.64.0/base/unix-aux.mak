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
# $Id: unix-aux.mak,v 1.12 2008/05/04 14:34:47 Arabidopsis Exp $
# Partial makefile common to all Unix configurations.
# This makefile contains the build rules for the auxiliary programs such as
# echogs, and the 'platform' modules.

# Define the name of this makefile.
UNIX_AUX_MAK=$(GLSRC)unix-aux.mak

# -------------------------------- Library -------------------------------- #

## The Unix platforms

# We have to include a test for the existence of sys/time.h,
# because some System V platforms don't have it.

# Unix platforms other than System V, and also System V Release 4
# (SVR4) platforms.
unix__=$(GLOBJ)gp_getnv.$(OBJ) $(GLOBJ)gp_upapr.$(OBJ) $(GLOBJ)gp_unix.$(OBJ) $(GLOBJ)gp_unifs.$(OBJ) $(GLOBJ)gp_unifn.$(OBJ) $(GLOBJ)gp_stdia.$(OBJ) $(GLOBJ)gp_unix_cache.$(OBJ)
$(GLGEN)unix_.dev: $(unix__) $(GLD)nosync.dev $(GLD)smd5.dev
	$(SETMOD) $(GLGEN)unix_ $(unix__) -include $(GLD)nosync
	$(ADDMOD) $(GLGEN)unix_ -include $(GLD)smd5

$(GLOBJ)gp_unix.$(OBJ): $(GLSRC)gp_unix.c $(AK)\
 $(pipe__h) $(string__h) $(time__h)\
 $(gx_h) $(gsexit_h) $(gp_h)
	$(GLCC) $(FONTCONFIG_CFLAGS) $(GLO_)gp_unix.$(OBJ) $(C_) $(GLSRC)gp_unix.c

$(GLOBJ)gp_unix_cache.$(OBJ): $(GLSRC)gp_unix_cache.c $(AK)\
 $(stdio__h) $(string__h) $(time__h) $(gconfigd_h) $(gp_h) $(md5_h)
	$(GLCC) $(GLO_)gp_unix_cache.$(OBJ) $(C_) $(GLSRC)gp_unix_cache.c

# assume all Unix platforms support unbuffered read
$(GLOBJ)gp_stdia.$(OBJ): $(GLSRC)gp_stdia.c $(AK)\
  $(stdio__h) $(time__h) $(unistd__h) $(gx_h) $(gp_h)
	$(GLCC) $(GLO_)gp_stdia.$(OBJ) $(C_) $(GLSRC)gp_stdia.c

# System V platforms other than SVR4, which lack some system calls,
# but have pipes.
sysv__=$(GLOBJ)gp_getnv.$(OBJ) $(GLOBJ)gp_upapr.$(OBJ) $(GLOBJ)gp_unix.$(OBJ) $(GLOBJ)gp_unifs.$(OBJ) $(GLOBJ)gp_unifn.$(OBJ) $(GLOBJ)gp_sysv.$(OBJ)
$(GLGEN)sysv_.dev: $(sysv__) $(GLD)nosync.dev
	$(SETMOD) $(GLGEN)sysv_ $(sysv__) -include $(GLD)nosync

$(GLOBJ)gp_sysv.$(OBJ): $(GLSRC)gp_sysv.c $(stdio__h) $(time__h) $(AK)
	$(GLCC) $(GLO_)gp_sysv.$(OBJ) $(C_) $(GLSRC)gp_sysv.c

# -------------------------- Auxiliary programs --------------------------- #

$(ECHOGS_XE): $(GLSRC)echogs.c $(AK) $(stdpre_h)
	$(CCAUX) $(I_)$(GLSRCDIR)$(_I) $(O_)$(ECHOGS_XE) $(GLSRC)echogs.c

# On the RS/6000 (at least), compiling genarch.c with gcc with -O
# produces a buggy executable.
$(GENARCH_XE): $(GLSRC)genarch.c $(AK) $(GENARCH_DEPS)
	$(CCAUX) $(I_)$(GLSRCDIR)$(_I) $(O_)$(GENARCH_XE) $(GLSRC)genarch.c

$(GENCONF_XE): $(GLSRC)genconf.c $(AK) $(GENCONF_DEPS)
	$(CCAUX) $(I_)$(GLSRCDIR)$(_I) $(O_)$(GENCONF_XE) $(GLSRC)genconf.c

$(GENDEV_XE): $(GLSRC)gendev.c $(AK) $(GENDEV_DEPS)
	$(CCAUX) $(I_)$(GLSRCDIR)$(_I) $(O_)$(GENDEV_XE) $(GLSRC)gendev.c

$(GENHT_XE): $(GLSRC)genht.c $(AK) $(GENHT_DEPS)
	$(CCAUX) $(GENHT_CFLAGS) $(O_)$(GENHT_XE) $(GLSRC)genht.c

$(GENINIT_XE): $(GLSRC)geninit.c $(AK) $(GENINIT_DEPS)
	$(CCAUX) $(I_)$(GLSRCDIR)$(_I) $(O_)$(GENINIT_XE) $(GLSRC)geninit.c

# If zlib is shared, mkromfs must depend on -lz

ifeq ($(SHARE_ZLIB),1)
MKROMFS_OBJS= $(GLOBJ)gscdefs.$(OBJ) $(GLOBJ)gsmisc.$(OBJ) \
 $(GLOBJ)gpmisc.$(OBJ) $(GLOBJ)gslibctx.$(OBJ) $(GLOBJ)gp_getnv.$(OBJ) \
 $(GLOBJ)gp_unix.$(OBJ) $(GLOBJ)gp_unifs.$(OBJ) $(GLOBJ)gp_unifn.$(OBJ) \
 $(GLOBJ)gp_stdia.$(OBJ) $(GLOBJ)gsutil.$(OBJ)
else
MKROMFS_OBJS= $(MKROMFS_ZLIB_OBJS) $(GLOBJ)gscdefs.$(OBJ) $(GLOBJ)gsmisc.$(OBJ) \
 $(GLOBJ)gpmisc.$(OBJ) $(GLOBJ)gslibctx.$(OBJ) $(GLOBJ)gp_getnv.$(OBJ) \
 $(GLOBJ)gp_unix.$(OBJ) $(GLOBJ)gp_unifs.$(OBJ) $(GLOBJ)gp_unifn.$(OBJ) \
 $(GLOBJ)gp_stdia.$(OBJ) $(GLOBJ)gsutil.$(OBJ)
endif

$(MKROMFS_XE): $(GLSRC)mkromfs.c $(MKROMFS_COMMON_DEPS) $(MKROMFS_OBJS)
	$(CCAUX) $(GENOPT) $(CFLAGS) $(I_)$(GLSRCDIR)$(_I) $(I_)$(GLOBJ)$(_I) $(I_)$(ZSRCDIR)$(_I) $(GLSRC)mkromfs.c $(O_)$(MKROMFS_XE) $(MKROMFS_OBJS) -lm $(EXTRALIBS)

# Query the environment to construct gconfig_.h.
# The "else true;" is required because Ultrix's implementation of sh -e
# terminates execution of a command if any error occurs, even if the command
# traps the error with ||.
INCLUDE=/usr/include
$(gconfig__h): $(UNIX_AUX_MAK) $(ECHOGS_XE)
	$(ECHOGS_XE) -w $(gconfig__h) -x 2f2a -s This file was generated automatically by unix-aux.mak. -s -x 2a2f
	if ( test -f $(INCLUDE)/dirent.h ); then $(ECHOGS_XE) -a $(gconfig__h) -x 23 define HAVE_DIRENT_H; else true; fi
	if ( test -f $(INCLUDE)/ndir.h ); then $(ECHOGS_XE) -a $(gconfig__h) -x 23 define HAVE_NDIR_H; else true; fi
	if ( test -f $(INCLUDE)/sys/dir.h ); then $(ECHOGS_XE) -a $(gconfig__h) -x 23 define HAVE_SYS_DIR_H; else true; fi
	if ( test -f $(INCLUDE)/sys/ndir.h ); then $(ECHOGS_XE) -a $(gconfig__h) -x 23 define HAVE_SYS_NDIR_H; else true; fi
	if ( test -f $(INCLUDE)/sys/time.h ); then $(ECHOGS_XE) -a $(gconfig__h) -x 23 define HAVE_SYS_TIME_H; else true; fi
	if ( test -f $(INCLUDE)/sys/times.h ); then $(ECHOGS_XE) -a $(gconfig__h) -x 23 define HAVE_SYS_TIMES_H; else true; fi
	if ( test -f $(JSRCDIR)/jmemsys.h); then true; else $(ECHOGS_XE) -a $(gconfig__h) -x 23 define DONT_HAVE_JMEMSYS_H; fi
