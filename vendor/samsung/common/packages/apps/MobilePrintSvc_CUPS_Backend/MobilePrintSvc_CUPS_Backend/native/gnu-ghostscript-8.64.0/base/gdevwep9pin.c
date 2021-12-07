/* Copyright (C) 1989, 1992, 1995 artofcode LLC.  All rights reserved.
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the version 2 of the GNU General Public
  License as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

*/

/*$Id: gdevwep9pin.c,v 1.4 2008/05/04 08:52:15 Arabidopsis Exp $*/
/*


  This code is derived from Okidata IBM compatible dot-matrix printer driver
  for Ghostscript named gdevokki.c.

  Thanks to people who are involved in that project.

  Change : Addition of the Page Length command.  

  Sahaya Darcius  Jun 01-2006 WeP Peripherals Pvt Ltd, Mysore, India
*/
 
 
#include "gdevprn.h"

/*
 * Valid values for X_DPI:
 *
 *     90, 120, 240
 *
 * The value specified at compile time is the default value used if the
 * user does not specify a resolution at runtime.
 */

#ifndef X_DPI
#  define X_DPI 120
#endif

/*
 * Valid values for Y_DPI:
 *
 *     72, 144
 *
 * The value specified at compile time is the default value used if the
 * user does not specify a resolution at runtime.
 */

#ifndef Y_DPI
#  define Y_DPI 72
#endif

/* The device descriptor */
static dev_proc_print_page(wep9pin_print_page);

/* WeP 9 Pin device */
const gx_device_printer far_data gs_wep9pin_device =
  prn_device(prn_std_procs, "wep9pin",
	DEFAULT_WIDTH_10THS, DEFAULT_HEIGHT_10THS,
	X_DPI, Y_DPI,
	0.0, 0.0, 0.0, 0.0,			/* margins */
	1, wep9pin_print_page);

/* ------ Internal routines ------ */

/* Forward references */
static void wep_output_run(byte *, int, int, char, FILE *, int);

/* Send the page to the printer. */
static int
wep9pin_print_page1(gx_device_printer *pdev, FILE *prn_stream, int y_9pin_high,
  const char *init_string, int init_length,
  const char *end_string, int end_length)
{	
	static const char graphics_modes_9[5] =
	{	
	-1, 0 /*90*/, 1 /*120*/, -1, 3 /*240*/
	};

	int in_y_mult = (y_9pin_high ? 2 : 1);
	int line_size = gdev_mem_bytes_per_scan_line((gx_device *)pdev);
	/* Note that in_size is a multiple of 8. */
	int in_size = line_size * (8 * in_y_mult);
	byte *buf1 = (byte *)gs_malloc(pdev->memory, in_size, 1, "wep9pin_print_page(buf1)");
	byte *buf2 = (byte *)gs_malloc(pdev->memory, in_size, 1, "wep9pin_print_page(buf2)");
	byte *in = buf1;
	byte *out = buf2;
	int out_y_mult = 1;
	int x_dpi = pdev->x_pixels_per_inch;
	char start_graphics = graphics_modes_9[x_dpi / 60];
	int first_pass = (start_graphics == 3 ? 1 : 0);
	int last_pass = first_pass * 2;
	int y_passes = (y_9pin_high ? 2 : 1);
	int skip = 0, lnum = 0, pass, ypass;
	int y_step = 0;
	float pl=0.0;

	/* Check allocations */
	if ( buf1 == 0 || buf2 == 0 )
	{	if ( buf1 ) 
		  gs_free(pdev->memory, (char *)buf1, in_size, 1, "wep9pin_print_page(buf1)");
		if ( buf2 ) 
		  gs_free(pdev->memory, (char *)buf2, in_size, 1, "wep9pin_print_page(buf2)");
		return_error(gs_error_VMerror);
	}

	/* Initialize the printer. */
	fwrite(init_string, 1, init_length, prn_stream);

	/* Page Length command is added*/
  	pl=(float)pdev->height/(float)pdev->y_pixels_per_inch;
	fprintf(prn_stream, "\033C%c", (int)(pl*6+0.5));
	
	/* Print lines of graphics */
	while ( lnum < pdev->height )
	{	
		byte *in_data;
		byte *inp;
		byte *in_end;
		byte *out_end;
		int lcnt;

		/* Copy 1 scan line and test for all zero. */
		gdev_prn_get_bits(pdev, lnum, in, &in_data);
		if ( in_data[0] == 0 &&
		     !memcmp((char *)in_data, (char *)in_data + 1, line_size - 1)
		   )
	    	{	
			lnum++;
			skip += 2 / in_y_mult;
			continue;
		}

		/*
		 * Vertical tab to the appropriate position.
		 * The skip count is in 1/144" steps.  If total
		 * vertical request is not a multiple od 1/72"
		 * we need to make sure the page is actually
		 * going to advance.
		 */
		if ( skip & 1 )
		{
			int n = 1 + (y_step == 0 ? 1 : 0);
			fprintf(prn_stream, "\033J%c", n);
			y_step = (y_step + n) % 3;
			skip -= 1;
		}
		skip = skip / 2 * 3;
		while ( skip > 255 )
		{	
			fputs("\033J\377", prn_stream);
			skip -= 255;
		}
		if ( skip )
		{
			fprintf(prn_stream, "\033J%c", skip);
		}

		/* Copy the the scan lines. */
	    	lcnt = gdev_prn_copy_scan_lines(pdev, lnum, in, in_size);
		if ( lcnt < 8 * in_y_mult )
		{	/* Pad with lines of zeros. */
			memset(in + lcnt * line_size, 0,
			       in_size - lcnt * line_size);
		}

		if ( y_9pin_high )
		{	/* Shuffle the scan lines */
			byte *p;
			int i;
			static const char index[] =
			{  0, 2, 4, 6, 8, 10, 12, 14,
			   1, 3, 5, 7, 9, 11, 13, 15
			};
			for ( i = 0; i < 16; i++ )
			{
				memcpy( out + (i * line_size),
				        in + (index[i] * line_size),
				        line_size);
			}
			p = in;
			in = out;
			out = p;
		}

	for ( ypass = 0; ypass < y_passes; ypass++ )
	{
	    for ( pass = first_pass; pass <= last_pass; pass++ )
	    {
		/* We have to 'transpose' blocks of 8 pixels x 8 lines, */
		/* because that's how the printer wants the data. */

	        if ( pass == first_pass )
	        {
		    out_end = out;
		    inp = in;
		    in_end = inp + line_size;
    
    	            for ( ; inp < in_end; inp++, out_end += 8 )
    	            { 
    		        gdev_prn_transpose_8x8(inp + (ypass * 8 * line_size), 
					       line_size, out_end, 1);
		    }
		    /* Remove trailing 0s. */
		    while ( out_end > out && out_end[-1] == 0 )
	            {
		       	out_end--;
		    }
		}

		/* Transfer whatever is left and print. */
		if ( out_end > out )
	        {
		    wep_output_run(out, (int)(out_end - out),
			           out_y_mult, start_graphics,
				   prn_stream, pass);
	        }
	    	fputc('\r', prn_stream);
	    }
	    if ( ypass < y_passes - 1 )
	    {
		int n = 1 + (y_step == 0 ? 1 : 0);
		fprintf(prn_stream, "\033J%c", n);
		y_step = (y_step + n) % 3;
	    }
	}
	skip = 16 - y_passes + 1;		/* no skip on last Y pass */
	lnum += 8 * in_y_mult;
	}

	/* Reinitialize the printer. */
	fwrite(end_string, 1, end_length, prn_stream);
	fflush(prn_stream);

	gs_free(pdev->memory, (char *)buf2, in_size, 1, "wep9pin_print_page(buf2)");
	gs_free(pdev->memory, (char *)buf1, in_size, 1, "wep9pin_print_page(buf1)");
	return 0;
}

/* Output a single graphics command. */
/* pass=0 for all columns, 1 for even columns, 2 for odd columns. */
static void
wep_output_run(byte *data, int count, int y_mult,
  char start_graphics, FILE *prn_stream, int pass)
{	
	int xcount = count / y_mult;

	fputc(033, prn_stream);
	fputc("KLYZ"[start_graphics], prn_stream);
	fputc(xcount & 0xff, prn_stream);
	fputc(xcount >> 8, prn_stream);
	if ( !pass )
	{
		fwrite(data, 1, count, prn_stream);
	}
	else
	{	
		/* Only write every other column of y_mult bytes. */
		int which = pass;
		register byte *dp = data;
		register int i, j;

		for ( i = 0; i < xcount; i++, which++ )
		{
			for ( j = 0; j < y_mult; j++, dp++ )
			{
				putc(((which & 1) ? *dp : 0), prn_stream);
			}
		}
	}
}

/* The print_page procedures are here, to avoid a forward reference. */

static const char wep_init_string[]	= { 0x18 };
static const char wep_end_string[]	= { 0x0c };

static int
wep9pin_print_page(gx_device_printer *pdev, FILE *prn_stream)
{
	char init_string[16], end_string[16];
	int init_length, end_length;

	init_length = sizeof(wep_init_string);
	memcpy(init_string, wep_init_string, init_length);

	end_length = sizeof(wep_end_string);
	memcpy(end_string, wep_end_string, end_length);

	
	return wep9pin_print_page1( pdev, prn_stream, 
				   pdev->y_pixels_per_inch > 72 ? 1 : 0,
				   init_string, init_length,
				   end_string, end_length );
}


