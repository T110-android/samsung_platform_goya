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

/* $Id: gp_os2.h,v 1.4 2007/09/11 15:24:01 Arabidopsis Exp $ */
/* Common platform-specific routines for OS/2 */

#ifndef gp_os2_INCLUDED
#  define gp_os2_INCLUDED

/* If queue_name is NULL, list available queues */
/* If strlen(queue_name)==0, return default queue and driver name */
/* If queue_name supplied, return driver_name */
/* returns 0 if OK, non-zero for error */
int pm_find_queue(char *queue_name, char *driver_name);

/* Spool file to queue */
/* Return 0 if successful, non-zero if error. */
/* If filename is NULL, return 0 if spool queue is valid, non-zero if error */
int pm_spool(char *filename, const char *queue);


#endif /* gp_os2_INCLUDED */
