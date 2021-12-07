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
/* $Id: gdevevga.c,v 1.7 2007/09/11 15:24:05 Arabidopsis Exp $ */
/* IBM PC EGA and VGA display drivers */
/* All of the real code is in gdevpcfb.c. */
#include "memory_.h"
#include "gx.h"
#include "gserrors.h"
#include "gxdevice.h"
#include "gdevpcfb.h"

/* ------ Internal routines ------ */

/* We can't catch signals.... */
void
pcfb_set_signals(gx_device * dev)
{
}

/* Read the device state */
void
pcfb_get_state(pcfb_bios_state * pbs)
{
    registers regs;

    regs.h.ah = 0xf;
    int86(0x10, &regs, &regs);
    pbs->display_mode = regs.h.al;
    pbs->text_page = regs.h.bh;
    regs.h.ah = 0x3;
    int86(0x10, &regs, &regs);
    pbs->text_cursor_mode = regs.rshort.cx;
    regs.rshort.ax = 0x1130;
    regs.h.bh = 0;
    int86(0x10, &regs, &regs);
    switch (regs.rshort.cx) {
	case 0x08:
	    pbs->text_font = 0x1112;
	    break;		/* 8 x 8 */
	case 0x10:
	    pbs->text_font = 0x1114;
	    break;		/* 8 x 16 */
	default:
	    pbs->text_font = 0x1111;	/* 8 x 14 */
    }
    regs.h.ah = 0x8;
    regs.h.bh = pbs->text_page;
    int86(0x10, &regs, &regs);
    pbs->text_attribute = regs.h.ah;
    pbs->border_color = (regs.h.ah >> 4);
    regs.rshort.ax = 0x1a00;
    int86(0x10, &regs, &regs);
    if (regs.h.al == 0x1a && regs.h.bl == 0x8) {
	regs.rshort.ax = 0x1008;
	int86(0x10, &regs, &regs);
	pbs->border_color = regs.h.bh;
    }
    if (pbs->display_mode != 3) {
	pbs->display_mode = 3;
	pbs->text_font = 0x1112;
	pbs->text_cursor_mode = 0x0607;
	pbs->text_attribute = 7;
	pbs->text_page = 0;
    }
}

/* Set the device mode */
void
pcfb_set_mode(int mode)
{
    registers regs;

    regs.h.ah = 0;
    regs.h.al = mode;
    int86(0x10, &regs, &regs);
}

/* Restore the device state */
void
pcfb_set_state(const pcfb_bios_state * pbs)
{
    registers regs;

    pcfb_set_mode(pbs->display_mode);
    regs.rshort.ax = 0x500;	/* force display of page 0 */
    int86(0x10, &regs, &regs);
    regs.rshort.ax = pbs->text_font;
    regs.h.bl = 0;
    int86(0x10, &regs, &regs);
    regs.h.ah = 0x3;
    regs.h.bh = 0;
    int86(0x10, &regs, &regs);	/* Get cursor to reset MCGA */
    regs.h.al = pbs->text_page;
    regs.h.ah = 0x5;
    int86(0x10, &regs, &regs);
    regs.rshort.cx = pbs->text_cursor_mode;
    regs.h.ah = 0x1;
    int86(0x10, &regs, &regs);
    regs.rshort.ax = 0x1001;
    regs.h.bh = pbs->border_color;
    int86(0x10, &regs, &regs);
}
