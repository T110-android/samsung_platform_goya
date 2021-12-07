/* Copyright (C) 2001-2006 Artifex Software, Inc.
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

/* $Id: gp_sysv.c,v 1.7 2007/09/11 15:24:08 Arabidopsis Exp $ */
/* System V Unix-specific routines for Ghostscript */

/* This file contains a couple of standard Unix library procedures */
/* that a few System V platforms don't provide. */
/* Note that this file is NOT used for SVR4 platforms. */
#include <errno.h>
#include "stdio_.h"
#include "time_.h"
#include <sys/types.h>
#include <sys/times.h>
#include <sys/stat.h>
#include <sys/param.h>

/* rename */
int
rename(const char *a, const char *b)
{
    if (access(a, 0) == -1)
	return (-1);
    unlink(b);
    if (link(a, b) == -1)
	return (-1);
    if (unlink(a) == -1) {
	unlink(b);		/* ??? */
	return (-1);
    }
    return (0);
}

/* gettimeofday */
#ifndef HZ
#  define	HZ	100	/* see sys/param.h */
#endif
int
gettimeofday(struct timeval *tvp, struct timezone *tzp)
{
    struct tms tms;
    static long offset = 0;
    long ticks;

    if (!offset) {
	time(&offset);
	offset -= (times(&tms) / HZ);
    }
    ticks = times(&tms);
    tvp->tv_sec = ticks / HZ + offset;
    tvp->tv_usec = (ticks % HZ) * (1000 * 1000 / HZ);
    return 0;
}
