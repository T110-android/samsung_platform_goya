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

/* $Id: gxclthrd.h,v 1.1 2009/04/19 13:54:31 Arabidopsis Exp $ */
/* Command list multiple rendering threads */
/* Requires gxsync.h */

#ifndef gxclthrd_INCLUDED
#  define gxclthrd_INCLUDED

#include "gxsync.h"

#define RENDER_THREAD_IDLE 0
#define RENDER_THREAD_DONE 1
#define RENDER_THREAD_BUSY 2

#ifndef clist_render_thread_control_t_DEFINED
#  define clist_render_thread_control_t_DEFINED
typedef struct clist_render_thread_control_s clist_render_thread_control_t;
#endif

struct clist_render_thread_control_s {
    int status;	/* 0: not started, 1: done, 2: busy, < 0: error */ 
		/* values allow waiting until status < 2 */
    gs_memory_t *memory;	/* thread's 'chunk' memory allocator */
    gx_semaphore_t *sema_this;
    gx_semaphore_t *sema_group;
    gx_device *cdev;	/* clist device copy */
    gx_device *bdev;	/* this thread's buffer device */
    int band;
#ifdef DEBUG
    ulong cputime;
#endif
};

#endif /* gxclthrd_INCLUDED */
