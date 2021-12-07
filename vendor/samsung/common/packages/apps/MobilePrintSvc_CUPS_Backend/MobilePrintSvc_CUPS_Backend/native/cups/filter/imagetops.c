/*
 * "$Id: imagetops.c 9901 2011-08-17 21:01:53Z mike $"
 *
 *   Image file to PostScript filter for CUPS.
 *
 *   Copyright 2007-2011 by Apple Inc.
 *   Copyright 1993-2007 by Easy Software Products.
 *
 *   These coded instructions, statements, and computer programs are the
 *   property of Apple Inc. and are protected by Federal copyright
 *   law.  Distribution and use rights are outlined in the file "LICENSE.txt"
 *   which should have been included with this file.  If this file is
 *   file is missing or damaged, see the license at "http://www.cups.org/".
 *
 *   This file is subject to the Apple OS-Developed Software exception.
 *
 * Contents:
 *
 *   main()       - Main entry...
 *   ps_hex()     - Print binary data as a series of hexadecimal numbers.
 *   ps_ascii85() - Print binary data as a series of base-85 numbers.
 */

/*
 * Include necessary headers...
 */

#include "common.h"
#include "image.h"
#include "imagetops.h"
#include <math.h>
#include <cups/language-private.h>
#include <signal.h>
#include<android/log.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "gs.h"

#define LOG_TAG "GenericPrintService"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)


/*
 * Globals...
 */

int	Flip ,		/* Flip/mirror pages */
	XPosition,		/* Horizontal position on page */
	YPosition,		/* Vertical position on page */
	Collate,		/* Collate copies? */
	Copies;		/* Number of copies */


char *ppdFilePath;
char *psFilePath;
char *ContentType;

/*
 * Local functions...
 */

static void	ps_hex(cups_ib_t *, int, int);
static void	ps_ascii85(cups_ib_t *, int, int);


/*
 * 'main()' - Main entry...
 */

int					/* O - Exit status */
imagetops_main(int  argc,				/* I - Number of command-line arguments */
     char *argv[])			/* I - Command-line arguments */
{
  cups_image_t	*img;			/* Image to print */
  float		xprint,			/* Printable area */
		yprint,
		xinches,		/* Total size in inches */
		yinches;
  float		xsize,			/* Total size in points */
		ysize,
		xsize2,
		ysize2;
  float		aspect;			/* Aspect ratio */
  int		xpages,			/* # x pages */
		ypages,			/* # y pages */
		xpage,			/* Current x page */
		ypage,			/* Current y page */
		page;			/* Current page number */
  int		xc0, yc0,			/* Corners of the page in image coords */
		xc1, yc1;
  cups_ib_t	*row;			/* Current row */
  int		y;			/* Current Y coordinate in image */
  int		colorspace;		/* Output colorspace */
  int		out_offset,		/* Offset into output buffer */
		out_length;		/* Length of output buffer */
  ppd_file_t	*ppd;			/* PPD file */
  ppd_choice_t	*choice;		/* PPD option choice */
  int		num_options;		/* Number of print options */
  cups_option_t	*options;		/* Print options */
  const char	*val;			/* Option value */
  int		slowcollate;		/* Collate copies the slow way */
  float		g;			/* Gamma correction value */
  float		b;			/* Brightness factor */
  float		zoom;			/* Zoom facter */
  int		xppi, yppi;		/* Pixels-per-inch */
  int		hue, sat;		/* Hue and saturation adjustment */
  int		realcopies,		/* Real copies being printed */
		emit_jcl;		/* Emit JCL? */
  float		left, top;		/* Left and top of image */
  char		filename[1024];		/* Name of file to print */
  time_t	curtime;		/* Current time */
  struct tm	*curtm;			/* Current date */
  char		curdate[255];		/* Current date string */
  int psFileMode = 0; //0:CREATE New ; 1: Append
  int outColorDevice = 1; //0 : monochrome print , 1:color print
  int count = 0,numFiles=1;
  int defout,outfd;
 /*
  * Make sure status messages are not buffered...
  */

	int ii=0;
	for(;ii<argc;ii++)
		LOGI("imagetops : argv:%s",argv[ii]);

  fflush(stdout);

  Copies = 1;	
  
  setbuf(stderr, NULL);

 /*
  * Ignore broken pipe signals...
  */

  signal(SIGPIPE, SIG_IGN);

 	
 /*
  * Process command-line options and write the prolog...
  */

  zoom = 0.0;
  xppi = 0;
  yppi = 0;
  hue  = 0;
  sat  = 100;
  g    = 1.0;
  b    = 1.0;

  Copies = atoi(argv[4]);

  options     = NULL;
  num_options = cupsParseOptions(argv[5], 0, &options);

  ppdFilePath = argv[6];
  ContentType= argv[7];
  psFilePath = argv[8];
  outColorDevice = atoi(argv[9]);
  numFiles = argc - 10;
  LOGI("imagetops : numFiles=%d",numFiles);
  
  
  ppd = SetCommonOptions(num_options, options, 0);
  
  if ((val = cupsGetOption("multiple-document-handling", num_options, options)) != NULL)
  {
   /*
    * This IPP attribute is unnecessarily complicated...
    *
    *   single-document, separate-documents-collated-copies, and
    *   single-document-new-sheet all require collated copies.
    *
    *   separate-documents-uncollated-copies allows for uncollated copies.
    */

    Collate = _cups_strcasecmp(val, "separate-documents-uncollated-copies") != 0;
  }

  if ((val = cupsGetOption("Collate", num_options, options)) != NULL &&
      _cups_strcasecmp(val, "True") == 0)
    Collate = 1;

  if ((val = cupsGetOption("gamma", num_options, options)) != NULL)
  {
   /*
    * Get gamma value from 1 to 10000...
    */

    g = atoi(val) * 0.001f;

    if (g < 0.001f)
      g = 0.001f;
    else if (g > 10.0f)
      g = 10.0f;
  }

  if ((val = cupsGetOption("brightness", num_options, options)) != NULL)
  {
   /*
    * Get brightness value from 10 to 1000.
    */

    b = atoi(val) * 0.01f;

    if (b < 0.1f)
      b = 0.1f;
    else if (b > 10.0f)
      b = 10.0f;
  }

  if ((val = cupsGetOption("scaling", num_options, options)) != NULL)
		zoom = atoi(val) * 0.01;
  else if ((val = cupsGetOption("fitplot", num_options, options)) != NULL &&
           !_cups_strcasecmp(val, "true"))
		zoom = 1.0;
  else if ((val = cupsGetOption("fit-to-page", num_options, options)) != NULL &&
           !_cups_strcasecmp(val, "true"))
    zoom = 1.0;

  if ((val = cupsGetOption("ppi", num_options, options)) != NULL)
    if (sscanf(val, "%dx%d", &xppi, &yppi) < 2)
      yppi = xppi;

  if ((val = cupsGetOption("position", num_options, options)) != NULL)
  {
    if (_cups_strcasecmp(val, "center") == 0)
    {
      XPosition = 0;
      YPosition = 0;
    }
    else if (_cups_strcasecmp(val, "top") == 0)
    {
      XPosition = 0;
      YPosition = 1;
    }
    else if (_cups_strcasecmp(val, "left") == 0)
    {
      XPosition = -1;
      YPosition = 0;
    }
    else if (_cups_strcasecmp(val, "right") == 0)
    {
      XPosition = 1;
      YPosition = 0;
    }
    else if (_cups_strcasecmp(val, "top-left") == 0)
    {
      XPosition = -1;
      YPosition = 1;
    }
    else if (_cups_strcasecmp(val, "top-right") == 0)
    {
      XPosition = 1;
      YPosition = 1;
    }
    else if (_cups_strcasecmp(val, "bottom") == 0)
    {
      XPosition = 0;
      YPosition = -1;
    }
    else if (_cups_strcasecmp(val, "bottom-left") == 0)
    {
      XPosition = -1;
      YPosition = -1;
    }
    else if (_cups_strcasecmp(val, "bottom-right") == 0)
    {
      XPosition = 1;
      YPosition = -1;
    }
  }

  if ((val = cupsGetOption("saturation", num_options, options)) != NULL)
    sat = atoi(val);

  if ((val = cupsGetOption("hue", num_options, options)) != NULL)
    hue = atoi(val);

  if ((choice = ppdFindMarkedChoice(ppd, "MirrorPrint")) != NULL)
  {
    val = choice->choice;
    choice->marked = 0;
  }
  else
    val = cupsGetOption("mirror", num_options, options);

  if (val && (!_cups_strcasecmp(val, "true") || !_cups_strcasecmp(val, "on") ||
              !_cups_strcasecmp(val, "yes")))
    Flip = 1;

  if ((val = cupsGetOption("emit-jcl", num_options, options)) != NULL &&
      (!_cups_strcasecmp(val, "false") || !_cups_strcasecmp(val, "off") ||
       !_cups_strcasecmp(val, "no") || !strcmp(val, "0")))
    emit_jcl = 0;
  else
    emit_jcl = 1;

  LOGI("imagetops/Copies=%d , Collate=%d, gamma=%f, brightness=%f, zoom=%f",Copies,Collate,g,b,zoom);
  LOGI("imagetops/xppi=%d,yppi=%d,XPosition=%d,YPosition=%d,sat=%d,hue=%d,flip=%d,emit_jcl=%d",xppi,yppi,XPosition,YPosition,sat,hue,Flip,emit_jcl);
 /*
  * Open the input image to print...
  */
  
	if(outColorDevice == 1) //added @ grv
		colorspace = CUPS_IMAGE_RGB_CMYK;
	else
		colorspace = CUPS_IMAGE_WHITE;

	LOGI("imagetops/ colorspace = %d",colorspace);
		
	for(count=0;count<numFiles;count++){
		Copies = atoi(argv[4]);
		//do everything for both image files
		strlcpy(filename, argv[(10 + count)], sizeof(filename)); 
		LOGI("imagetops : count=%d , filename=%s",count,filename);
		img = cupsImageOpen(filename, colorspace, CUPS_IMAGE_WHITE, sat, hue, NULL);
		if (img == NULL)
		{
			LOGI("imagetops:error:The print file could not be opened.");
			_cupsLangPrintFilter(stderr, "ERROR",
							 _("The print file could not be opened."));
			ppdClose(ppd);
			return (1);
		}
		colorspace = cupsImageGetColorSpace(img);
		LOGI("imagetops: cupsImageGetColorSpace finished : colorspace :%d",colorspace);
		/*
		* Scale as necessary...
		*/
			
		if (zoom == 0.0 && xppi == 0)
		{
			xppi = cupsImageGetXPPI(img);
			yppi = cupsImageGetYPPI(img);
		}
		LOGI("imagetops:xppi=%d",xppi);
		LOGI("imagetops:yppi=%d",yppi);
		if (yppi == 0)
			yppi = xppi;

		fprintf(stderr, "DEBUG: Before scaling: xppi=%d, yppi=%d, zoom=%.2f\n",
			  xppi, yppi, zoom);
		if (xppi > 0)
		{
			/*
			* Scale the image as neccesary to match the desired pixels-per-inch.
			*/

			if (Orientation & 1)
			{
				xprint = (PageTop - PageBottom) / 72.0;
				yprint = (PageRight - PageLeft) / 72.0;
			}
			else
			{
				xprint = (PageRight - PageLeft) / 72.0;
				yprint = (PageTop - PageBottom) / 72.0;
			}

			fprintf(stderr, "DEBUG: Before scaling: xprint=%.1f, yprint=%.1f\n",
				xprint, yprint);

			xinches = (float)cupsImageGetWidth(img) / (float)xppi;
			yinches = (float)cupsImageGetHeight(img) / (float)yppi;

			fprintf(stderr, "DEBUG: Image size is %.1f x %.1f inches...\n",
				xinches, yinches);

			if ((val = cupsGetOption("natural-scaling", num_options, options)) != NULL)
			{
				xinches = xinches * atoi(val) / 100;
				yinches = yinches * atoi(val) / 100;
			}

			if (cupsGetOption("orientation-requested", num_options, options) == NULL &&
				cupsGetOption("landscape", num_options, options) == NULL)
			{
				/*
				* Rotate the image if it will fit landscape but not portrait...
				*/

				fputs("DEBUG: Auto orientation...\n", stderr);
				LOGI("imagetops.c : Auto orientation...");
				if ((xinches > xprint || yinches > yprint) && xinches <= yprint && yinches <= xprint)
				{
					/*
					* Rotate the image as needed...
					*/

					fputs("DEBUG: Using landscape orientation...\n", stderr);
					LOGI("imagetops.c : Auto orientation : Using landscape orientation");
					Orientation = (Orientation + 1) & 3;
					xsize       = yprint;
					yprint      = xprint;
					xprint      = xsize;
				}
			}
		}else{
			/*
			* Scale percentage of page size...
			*/

			xprint = (PageRight - PageLeft) / 72.0;
			yprint = (PageTop - PageBottom) / 72.0;
			aspect = (float)cupsImageGetYPPI(img) / (float)cupsImageGetXPPI(img);

			fprintf(stderr, "DEBUG: Before scaling: xprint=%.1f, yprint=%.1f\n",
            xprint, yprint);

			fprintf(stderr, "DEBUG: cupsImageGetXPPI(img) = %d, cupsImageGetYPPI(img) = %d, aspect = %f\n",
            cupsImageGetXPPI(img), cupsImageGetYPPI(img), aspect);

			xsize = xprint * zoom;
			ysize = xsize * cupsImageGetHeight(img) / cupsImageGetWidth(img) / aspect;

			if (ysize > (yprint * zoom))
			{
				ysize = yprint * zoom;
				xsize = ysize * cupsImageGetWidth(img) * aspect / cupsImageGetHeight(img);
			}

			xsize2 = yprint * zoom;
			ysize2 = xsize2 * cupsImageGetHeight(img) / cupsImageGetWidth(img) / aspect;

			if (ysize2 > (xprint * zoom))
			{
				ysize2 = xprint * zoom;
				xsize2 = ysize2 * cupsImageGetWidth(img) * aspect / cupsImageGetHeight(img);
			}

			fprintf(stderr, "DEBUG: Portrait size is %.2f x %.2f inches\n", xsize, ysize);
			fprintf(stderr, "DEBUG: Landscape size is %.2f x %.2f inches\n", xsize2, ysize2);

			if (cupsGetOption("orientation-requested", num_options, options) == NULL &&	cupsGetOption("landscape", num_options, options) == NULL)
			{
				/*
				* Choose the rotation with the largest area, but prefer
				* portrait if they are equal...
				*/

				fputs("DEBUG: Auto orientation...\n", stderr);

				if ((xsize * ysize) < (xsize2 * xsize2))
				{
					/*
					* Do landscape orientation...
					*/

					fputs("DEBUG: Using landscape orientation...\n", stderr);

					Orientation = 1;
					xinches     = xsize2;
					yinches     = ysize2;
					xprint      = (PageTop - PageBottom) / 72.0;
					yprint      = (PageRight - PageLeft) / 72.0;
				}
				else
				{
					/*
					* Do portrait orientation...
					*/

					fputs("DEBUG: Using portrait orientation...\n", stderr);

					Orientation = 0;
					xinches     = xsize;
					yinches     = ysize;
				}
			}
			else if (Orientation & 1)
			{
				fputs("DEBUG: Using landscape orientation...\n", stderr);

				xinches     = xsize2;
				yinches     = ysize2;
				xprint      = (PageTop - PageBottom) / 72.0;
				yprint      = (PageRight - PageLeft) / 72.0;
			}
			else
			{
				fputs("DEBUG: Using portrait orientation...\n", stderr);

				xinches     = xsize;
				yinches     = ysize;
				xprint      = (PageRight - PageLeft) / 72.0;
				yprint      = (PageTop - PageBottom) / 72.0;
			}
		}
		
		/*
		* Compute the number of pages to print and the size of the image on each
		* page...
		*/

		xpages = ceil(xinches / xprint);
		ypages = ceil(yinches / yprint);

		xprint = xinches / xpages;
		yprint = yinches / ypages;

		fprintf(stderr, "DEBUG: xpages = %dx%.2fin, ypages = %dx%.2fin\n",xpages, xprint, ypages, yprint);
		
		 
		/*
		* Update the page size for custom sizes...
		*/

		if ((choice = ppdFindMarkedChoice(ppd, "PageSize")) != NULL && _cups_strcasecmp(choice->choice, "Custom") == 0)
		{
			float	width,		/* New width in points */
			length;		/* New length in points */
			char	s[255];		/* New custom page size... */


			/*
			* Use the correct width and length for the current orientation...
			*/

			if (Orientation & 1)
			{
				width  = yprint * 72.0;
				length = xprint * 72.0;
			}
			else
			{
				width  = xprint * 72.0;
				length = yprint * 72.0;
			}

			/*
			* Add margins to page size...
			*/

			width  += ppd->custom_margins[0] + ppd->custom_margins[2];
			length += ppd->custom_margins[1] + ppd->custom_margins[3];

			/*
			* Enforce minimums...
			*/

			if (width < ppd->custom_min[0])
				width = ppd->custom_min[0];

			if (length < ppd->custom_min[1])
				length = ppd->custom_min[1];

			fprintf(stderr, "DEBUG: Updated custom page size to %.2f x %.2f inches...\n",width / 72.0, length / 72.0);

			/*
			* Set the new custom size...
			*/

			sprintf(s, "Custom.%.0fx%.0f", width, length);
			ppdMarkOption(ppd, "PageSize", s);

			/*
			* Update page variables...
			*/

			PageWidth  = width;
			PageLength = length;
			PageLeft   = ppd->custom_margins[0];
			PageRight  = width - ppd->custom_margins[2];
			PageBottom = ppd->custom_margins[1];
			PageTop    = length - ppd->custom_margins[3];
		}

		/*
		* See if we need to collate, and if so how we need to do it...
		*/

		if (xpages == 1 && ypages == 1)
			Collate = 0;

		slowcollate = Collate && ppdFindOption(ppd, "Collate") == NULL;

		if (Copies > 1 && !slowcollate)
		{
			realcopies = Copies;
			Copies     = 1;
		}
		else
			realcopies = 1;

		/*
		* Write any "exit server" options that have been selected...
		*/

		//Changes to create ps file -- start
		
		if(count == 0){//single side print
			defout = dup(1);
			outfd = open(psFilePath,O_RDWR | O_CREAT,S_IRWXU |S_IRWXG | S_IRWXO);
			dup2(outfd,1);
		}
		//Changes to create ps file -- end
		ppdEmit(ppd, stdout, PPD_ORDER_EXIT);
		/*
		* Write any JCL commands that are needed to print PostScript code...
		*/

		if (emit_jcl)
			ppdEmitJCL(ppd, stdout, atoi(argv[1]), argv[2], argv[3]);

		/*
		* Start sending the document with any commands needed...
		*/
		  
		curtime = time(NULL);
		curtm   = localtime(&curtime);
		//////////////////
		if(count == 0){
			puts("%!PS-Adobe-3.0");
			printf("%%%%BoundingBox: %.0f %.0f %.0f %.0f\n", PageLeft, PageBottom,PageRight, PageTop);
			printf("%%%%LanguageLevel: %d\n", LanguageLevel);
		
			printf("%%%%Pages: %d\n", xpages * ypages * Copies * numFiles);
			puts("%%DocumentData: Clean7Bit");
			puts("%%DocumentNeededResources: font Helvetica-Bold");
			puts("%%Creator: imagetops/" CUPS_SVERSION);
			strftime(curdate, sizeof(curdate), "%c", curtm);
			printf("%%%%CreationDate: %s\n", curdate);
			WriteTextComment("Title", argv[3]);
			WriteTextComment("For", argv[2]);
			if (Orientation & 1)
				puts("%%Orientation: Landscape");
			else
				puts("%%Orientation: Portrait");
			puts("%%EndComments");
			puts("%%BeginProlog");

			if (ppd != NULL && ppd->patches != NULL)
				puts(ppd->patches);

			ppdEmit(ppd, stdout, PPD_ORDER_DOCUMENT);
			ppdEmit(ppd, stdout, PPD_ORDER_ANY);
			ppdEmit(ppd, stdout, PPD_ORDER_PROLOG);

			if (g != 1.0 || b != 1.0)
				printf("{ neg 1 add dup 0 lt { pop 1 } { %.3f exp neg 1 add } "
					"ifelse %.3f mul } bind settransfer\n", g, b);

			WriteCommon();
			switch (Orientation)
			{
				case 0 :
					WriteLabelProlog(cupsGetOption("page-label", num_options, options),
						PageBottom, PageTop, PageWidth);
				break;

				case 1 :
					WriteLabelProlog(cupsGetOption("page-label", num_options, options),
						 PageLeft, PageRight, PageLength);
				break;

				case 2 :
					WriteLabelProlog(cupsGetOption("page-label", num_options, options),
						 PageLength - PageTop, PageLength - PageBottom, PageWidth);
				break;

				case 3 :
					WriteLabelProlog(cupsGetOption("page-label", num_options, options),
						 PageWidth - PageRight, PageWidth - PageLeft,PageLength);
				break;
			}

			if (realcopies > 1)
			{
				if (ppd == NULL || ppd->language_level == 1)
					printf("/#copies %d def\n", realcopies);
				else
					printf("<</NumCopies %d>>setpagedevice\n", realcopies);
			}

			puts("%%EndProlog");
		}
		/*
		* Output the pages...
		*/

		row = malloc(cupsImageGetWidth(img) * abs(colorspace) + 3);

		fprintf(stderr, "DEBUG: XPosition=%d, YPosition=%d, Orientation=%d\n",XPosition, YPosition, Orientation);
		fprintf(stderr, "DEBUG: xprint=%.0f, yprint=%.0f\n", xprint, yprint);
		fprintf(stderr, "DEBUG: PageLeft=%.0f, PageRight=%.0f, PageWidth=%.0f\n",PageLeft, PageRight, PageWidth);
		fprintf(stderr, "DEBUG: PageBottom=%.0f, PageTop=%.0f, PageLength=%.0f\n",PageBottom, PageTop, PageLength);

		switch (Orientation)
		{
			default :
				switch (XPosition)
				{
					case -1 :
						left = PageLeft;
					break;
					default :
						left = (PageRight + PageLeft - xprint * 72) / 2;
					break;
					case 1 :
						left = PageRight - xprint * 72;
					break;
				}

				switch (YPosition)
				{
					case -1 :
						top = PageBottom + yprint * 72;
					break;
					default :
						top = (PageTop + PageBottom + yprint * 72) / 2;
					break;
					case 1 :
						top = PageTop;
					break;
				}
			break;

			case 1 :
				switch (XPosition)
				{
					case -1 :
						left = PageBottom;
					break;
					default :
						left = (PageTop + PageBottom - xprint * 72) / 2;
					break;
					case 1 :
						left = PageTop - xprint * 72;
					break;
				}

				switch (YPosition)
				{
					case -1 :
						top = PageLeft + yprint * 72;
					break;
					default :
						top = (PageRight + PageLeft + yprint * 72) / 2;
					break;
					case 1 :
						top = PageRight;
					break;
				}
			break;

			case 2 :
				switch (XPosition)
				{
					case 1 :
						left = PageLeft;
					break;
					default :
						left = (PageRight + PageLeft - xprint * 72) / 2;
					break;
					case -1 :
						left = PageRight - xprint * 72;
					break;
				}

				switch (YPosition)
				{
					case 1 :
						top = PageBottom + yprint * 72;
					break;
					default :
						top = (PageTop + PageBottom + yprint * 72) / 2;
					break;
					case -1 :
						top = PageTop;
					break;
				}
			break;

			case 3 :
				switch (XPosition)
				{
					case 1 :
						left = PageBottom;
					break;
					default :
						left = (PageTop + PageBottom - xprint * 72) / 2;
					break;
					case -1 :
						left = PageTop - xprint * 72;
					break;
				}

				switch (YPosition)
				{
					case 1 :
						top = PageLeft + yprint * 72;
					break;
					default :
						top = (PageRight + PageLeft + yprint * 72) / 2;
					break;
					case -1 :
						top = PageRight;
					break;
				}
			break;
		}

		fprintf(stderr, "DEBUG: left=%.2f, top=%.2f\n", left, top);
		
		for (page = 1; Copies > 0; Copies --)
			for (xpage = 0; xpage < xpages; xpage ++)
				for (ypage = 0; ypage < ypages; ypage ++, page ++)
				{
					if (ppd && ppd->num_filters == 0)
						fprintf(stderr, "PAGE: %d %d\n", page, realcopies);

					_cupsLangPrintFilter(stderr, "INFO", _("Printing page %d."), page);

					printf("%%%%Page: %d %d\n", (page * (count+1)), (page*(count+1)));
					//TEST_TODO
					if(count>=1){
						puts("%%BeginPageSetup");
						if (Orientation & 1)
							puts("%%PageOrientation: Landscape");
						else
							puts("%%PageOrientation: Portrait");
						puts("%%EndPageSetup");
					}
					ppdEmit(ppd, stdout, PPD_ORDER_PAGE);

					puts("gsave");

					if (Flip)
						printf("%.0f 0 translate -1 1 scale\n", PageWidth);

					switch (Orientation)
					{
						case 1 : /* Landscape */
							printf("%.0f 0 translate 90 rotate\n", PageWidth);
						break;
						case 2 : /* Reverse Portrait */
							printf("%.0f %.0f translate 180 rotate\n", PageWidth, PageLength);
						break;
						case 3 : /* Reverse Landscape */
							printf("0 %.0f translate -90 rotate\n", PageLength);
						break;
					}

					puts("gsave");

					xc0 = cupsImageGetWidth(img) * xpage / xpages;
					xc1 = cupsImageGetWidth(img) * (xpage + 1) / xpages - 1;
					yc0 = cupsImageGetHeight(img) * ypage / ypages;
					yc1 = cupsImageGetHeight(img) * (ypage + 1) / ypages - 1;

					printf("%.1f %.1f translate\n", left, top);

					printf("%.3f %.3f scale\n\n",xprint * 72.0 / (xc1 - xc0 + 1),yprint * 72.0 / (yc1 - yc0 + 1));

					if (LanguageLevel == 1)
					{
						printf("/picture %d string def\n", (xc1 - xc0 + 1) * abs(colorspace));
						printf("%d %d 8[1 0 0 -1 0 1]", (xc1 - xc0 + 1), (yc1 - yc0 + 1));

						if (colorspace == CUPS_IMAGE_WHITE)
							puts("{currentfile picture readhexstring pop} image");
						else
							printf("{currentfile picture readhexstring pop} false %d colorimage\n",abs(colorspace));

						for (y = yc0; y <= yc1; y ++)
						{
							cupsImageGetRow(img, xc0, y, xc1 - xc0 + 1, row);
							ps_hex(row, (xc1 - xc0 + 1) * abs(colorspace), y == yc1);
						}
					}
					else
					{
						switch (colorspace)
						{
							case CUPS_IMAGE_WHITE :
								puts("/DeviceGray setcolorspace");
							break;
							case CUPS_IMAGE_RGB :
								puts("/DeviceRGB setcolorspace");
							break;
							case CUPS_IMAGE_CMYK :
								puts("/DeviceCMYK setcolorspace");
							break;
						}

						printf("<<"
							"/ImageType 1"
							"/Width %d"
							"/Height %d"
							"/BitsPerComponent 8",
							xc1 - xc0 + 1, yc1 - yc0 + 1);

						switch (colorspace)
						{
							case CUPS_IMAGE_WHITE :
								fputs("/Decode[0 1]", stdout);
							break;
							case CUPS_IMAGE_RGB :
								fputs("/Decode[0 1 0 1 0 1]", stdout);
							break;
							case CUPS_IMAGE_CMYK :
								fputs("/Decode[0 1 0 1 0 1 0 1]", stdout);
							break;
						}

						fputs("\n/DataSource currentfile/ASCII85Decode filter", stdout);

						if (((xc1 - xc0 + 1) / xprint) < 100.0)
							fputs("/Interpolate true", stdout);

						puts("/ImageMatrix[1 0 0 -1 0 1]>>image");

						for (y = yc0, out_offset = 0; y <= yc1; y ++)
						{
							cupsImageGetRow(img, xc0, y, xc1 - xc0 + 1, row + out_offset);

							out_length = (xc1 - xc0 + 1) * abs(colorspace) + out_offset;
							out_offset = out_length & 3;

							ps_ascii85(row, out_length, y == yc1);

							if (out_offset > 0)
								memcpy(row, row + out_length - out_offset, out_offset);
						}
					}

					if(count==1){
						puts("grestore");
						WriteLabels(0);
						puts("grestore");
					}
					puts("showpage");
					puts("%%PageTrailer");
				}
		
	} //end of big for loop
  
   
   	puts("%%EOF");
 
	/*
	* End the job with the appropriate JCL command or CTRL-D otherwise.
	*/

	if (emit_jcl)
	{
		if (ppd && ppd->jcl_end)
			ppdEmitJCLEnd(ppd, stdout);
		else
		putchar(0x04);
	}

	fflush(stdout);
	dup2(defout,1);
	close(outfd);
	close(defout);

	/*
	* Close files...
	*/
	LOGI("imagetops:Close files");
	cupsImageClose(img);
	ppdClose(ppd);

  return (0);
}


/*
 * 'ps_hex()' - Print binary data as a series of hexadecimal numbers.
 */

static void
ps_hex(cups_ib_t *data,			/* I - Data to print */
       int       length,		/* I - Number of bytes to print */
       int       last_line)		/* I - Last line of raster data? */
{
  static int	col = 0;		/* Current column */
  static char	*hex = "0123456789ABCDEF";
					/* Hex digits */


  while (length > 0)
  {
   /*
    * Put the hex chars out to the file; note that we don't use printf()
    * for speed reasons...
    */

    putchar(hex[*data >> 4]);
    putchar(hex[*data & 15]);

    data ++;
    length --;

    col += 2;
    if (col > 78)
    {
      putchar('\n');
      col = 0;
    }
  }

  if (last_line && col)
  {
    putchar('\n');
    col = 0;
  }
}


/*
 * 'ps_ascii85()' - Print binary data as a series of base-85 numbers.
 */

static void
ps_ascii85(cups_ib_t *data,		/* I - Data to print */
	   int       length,		/* I - Number of bytes to print */
	   int       last_line)		/* I - Last line of raster data? */
{
  unsigned	b;			/* Binary data word */
  unsigned char	c[5];			/* ASCII85 encoded chars */
  static int	col = 0;		/* Current column */


  while (length > 3)
  {
    b = (((((data[0] << 8) | data[1]) << 8) | data[2]) << 8) | data[3];

    if (b == 0)
    {
      putchar('z');
      col ++;
    }
    else
    {
      c[4] = (b % 85) + '!';
      b /= 85;
      c[3] = (b % 85) + '!';
      b /= 85;
      c[2] = (b % 85) + '!';
      b /= 85;
      c[1] = (b % 85) + '!';
      b /= 85;
      c[0] = b + '!';

      fwrite(c, 5, 1, stdout);
      col += 5;
    }

    data += 4;
    length -= 4;

    if (col >= 75)
    {
      putchar('\n');
      col = 0;
    }
  }

  if (last_line)
  {
    if (length > 0)
    {
      memset(data + length, 0, 4 - length);
      b = (((((data[0] << 8) | data[1]) << 8) | data[2]) << 8) | data[3];

      c[4] = (b % 85) + '!';
      b /= 85;
      c[3] = (b % 85) + '!';
      b /= 85;
      c[2] = (b % 85) + '!';
      b /= 85;
      c[1] = (b % 85) + '!';
      b /= 85;
      c[0] = b + '!';

      fwrite(c, length + 1, 1, stdout);
    }

    puts("~>");
    col = 0;
  }
}


/*
 * End of "$Id: imagetops.c 9901 2011-08-17 21:01:53Z mike $".
 */
