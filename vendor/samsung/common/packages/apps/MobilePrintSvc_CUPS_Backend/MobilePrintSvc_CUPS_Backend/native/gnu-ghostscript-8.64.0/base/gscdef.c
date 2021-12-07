/* Copyright (C) 2001-2008 Artifex Software, Inc.
   All Rights Reserved.
  
  This file is part of GNU ghostscript

  GNU ghostscript is free software; you can redistribute it and/or
  modify it under the terms of the version 2 of the GNU General Public
  License as published by the Free Software Foundation.

  GNU ghostscript is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with
  ghostscript; see the file COPYING. If not, write to the Free Software Foundation,
  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

*/

/* $Id: gscdef.c,v 1.14 2008/05/04 21:04:04 Arabidopsis Exp $ */
/* Configuration scalars */

#include "std.h"
#include "gscdefs.h"		/* interface */
#include "gconfigd.h"		/* for #defines */

/* ---------------- Miscellaneous system parameters ---------------- */

/* All of these can be set in the makefile. */
/* Normally they are all const; see gscdefs.h for more information. */

#ifndef GS_BUILDTIME
#  define GS_BUILDTIME\
	0			/* should be set in the makefile */
#endif
const long gs_buildtime = GS_BUILDTIME;

#ifndef GS_COPYRIGHT
#  define GS_COPYRIGHT\
	"Copyright (C) 2009 Artifex Software, Inc.  All rights reserved."
#endif
const char *const gs_copyright = GS_COPYRIGHT;

#ifndef GS_PRODUCTFAMILY
#  define GS_PRODUCTFAMILY\
	"GNU Ghostscript"
#endif
const char *const gs_productfamily = GS_PRODUCTFAMILY;

#ifndef GS_PRODUCT
#  define GS_PRODUCT\
	GS_PRODUCTFAMILY ""
#endif
const char *const gs_product = GS_PRODUCT;

const char *
gs_program_name(void)
{
    return gs_product;
}

/* GS_VERSION must be defined in the Makefile. */
const long gs_version = GS_VERSION;

long gs_version_number(void)
{
	return gs_version;
}

/* GS_REVISION must be defined in the makefile. */
const long gs_revision = GS_REVISION;

long
gs_revision_number(void)
{
    return gs_revision;
}

/* GS_REVISIONDATE must be defined in the makefile. */
const long gs_revisiondate = GS_REVISIONDATE;

#ifndef GS_SERIALNUMBER
#  define GS_SERIALNUMBER\
	42			/* a famous number */
#endif
const long gs_serialnumber = GS_SERIALNUMBER;

/* ---------------- Installation directories and files ---------------- */

/* Here is where the library search path, the name of the */
/* initialization file, and the doc directory are defined. */
/* Define the documentation directory (only used in help messages). */
const char *const gs_doc_directory = GS_DOCDIR;

/* Define the default library search path. */
const char *const gs_lib_default_path = GS_LIB_DEFAULT;

/* Define the interpreter initialization file. */
const char *const gs_init_file = GS_INIT;

/* Define the default devices list. */
#ifndef GS_DEV_DEFAULT
#define GS_DEV_DEFAULT ""
#endif
const char *const gs_dev_defaults = GS_DEV_DEFAULT;
