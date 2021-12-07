/*
 *  Copyright (C) 2002-2008  Mattia Dongili <malattia@linux.it>
 *                           George Staikos <staikos@0wned.org>
 *  Copyright (C) 2012 Marvell, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __PPD_H__
#define __PPD_H__

#define __PPD_VERSION__    "1.0"
#define __PPD_MAINTAINER__	"malattia@linux.it"

#ifdef __GNUC__
#  define __UNUSED__	__attribute__((unused))
#else
#  define __UNUSED__
#endif

#ifndef PPD_CONFDIR
#  define PPD_CONFDIR  "/etc/"
#endif

#ifndef PPD_LIBDIR
#  define PPD_LIBDIR   "/system/lib/PowerDaemon/"
#endif

#ifndef PPD_STATEDIR
#  define PPD_STATEDIR   "/tmp/"
#endif

#define PPD_CONFIG		PPD_CONFDIR"powerdaemon.xml"
#define PPD_PIDFILE	PPD_STATEDIR"ppd.pid"
#define PPD_SOCKFILE	"/tmp/ppd.sock"

#define PPD_SOCKET_SVR "ppd_svr.socket"

#define DEFAULT_POLL		15
#define DEFAULT_VERBOSITY	5

#define MAX_STRING_LEN		255

#ifdef HAVE_LIMITS_H
#include <limits.h>
#define MAX_PATH_LEN PATH_MAX
#else
#define MAX_PATH_LEN 512
#endif

/* chip revision */
enum {
	PXA988_Z1,
	PXA988_Z2,
	PXA988_Z3,
};

#endif /* __PPD_H__ */
