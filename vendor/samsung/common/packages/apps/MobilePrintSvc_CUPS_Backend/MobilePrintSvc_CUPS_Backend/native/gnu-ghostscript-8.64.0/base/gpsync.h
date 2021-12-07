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

/* $Id: gpsync.h,v 1.7 2007/09/11 15:23:45 Arabidopsis Exp $ */
/* Interface to platform-dependent synchronization primitives */

#if !defined(gpsync_INCLUDED)
#  define gpsync_INCLUDED

/* Initial version 4/1/98 by John Desrosiers (soho@crl.com). */
/* 8/9/98 L. Peter Deutsch (ghost@aladdin.com) Changed ...sizeof to
   procedures, added some comments. */

/* -------- Synchronization primitives ------- */

/*
 * Semaphores support wait/signal semantics: a wait operation will allow
 * control to proceed iff the number of signals since semaphore creation
 * is greater than the number of waits.
 */
typedef struct {
    void *dummy_;
} gp_semaphore;

uint gp_semaphore_sizeof(void);
/*
 * Hack: gp_semaphore_open(0) succeeds iff it's OK for the memory manager
 * to move a gp_semaphore in memory.
 */
int gp_semaphore_open(gp_semaphore * sema);
int gp_semaphore_close(gp_semaphore * sema);
int gp_semaphore_wait(gp_semaphore * sema);
int gp_semaphore_signal(gp_semaphore * sema);

/*
 * Monitors support enter/leave semantics: at most one thread can have
 * entered and not yet left a given monitor.
 */
typedef struct {
    void *dummy_;
} gp_monitor;

uint gp_monitor_sizeof(void);
/*
 * Hack: gp_monitor_open(0) succeeds iff it's OK for the memory manager
 * to move a gp_monitor in memory.
 */
int gp_monitor_open(gp_monitor * mon);
int gp_monitor_close(gp_monitor * mon);
int gp_monitor_enter(gp_monitor * mon);
int gp_monitor_leave(gp_monitor * mon);

/*
 * A new thread starts by calling a procedure, passing it a void * that
 * allows it to gain access to whatever data it needs.
 */
typedef void (*gp_thread_creation_callback_t) (void *);
int gp_create_thread(gp_thread_creation_callback_t, void *);

#endif /* !defined(gpsync_INCLUDED) */
