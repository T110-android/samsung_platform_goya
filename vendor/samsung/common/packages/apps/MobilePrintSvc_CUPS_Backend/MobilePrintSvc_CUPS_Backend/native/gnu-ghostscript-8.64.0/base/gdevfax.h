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
/* $Id: gdevfax.h,v 1.7 2007/09/11 15:23:51 Arabidopsis Exp $ */
/* Definitions and interface for fax devices */

#ifndef gdevfax_INCLUDED
#  define gdevfax_INCLUDED

/* Define the default device parameters. */
#define X_DPI 204
#define Y_DPI 196

/* Define the structure for fax devices. */
/* Precede this by gx_device_common and gx_prn_device_common. */
#define gx_fax_device_common\
    int AdjustWidth		/* 0 = no adjust, 1 = adjust to fax values */
typedef struct gx_device_fax_s {
    gx_device_common;
    gx_prn_device_common;
    gx_fax_device_common;
} gx_device_fax;

#define FAX_DEVICE_BODY(dtype, procs, dname, print_page)\
    prn_device_std_body(dtype, procs, dname,\
			DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS,\
			X_DPI, Y_DPI,\
			0, 0, 0, 0,	/* margins */\
			1, print_page),\
    1				/* AdjustWidth */

/* Procedures defined in gdevfax.c */

/* Driver procedures */
dev_proc_open_device(gdev_fax_open);
dev_proc_get_params(gdev_fax_get_params); /* adds AdjustWidth */
dev_proc_put_params(gdev_fax_put_params); /* adds AdjustWidth */
extern const gx_device_procs gdev_fax_std_procs;

/* Other procedures */
void gdev_fax_init_state(stream_CFE_state *ss, const gx_device_fax *fdev);
void gdev_fax_init_fax_state(stream_CFE_state *ss,
			     const gx_device_fax *fdev);
int gdev_fax_print_strip(gx_device_printer * pdev, FILE * prn_stream,
			 const stream_template * temp, stream_state * ss,
			 int width, int row_first,
			 int row_end /* last + 1 */);
int gdev_fax_print_page(gx_device_printer *pdev, FILE *prn_stream,
			stream_CFE_state *ss);

#endif /* gdevfax_INCLUDED */
