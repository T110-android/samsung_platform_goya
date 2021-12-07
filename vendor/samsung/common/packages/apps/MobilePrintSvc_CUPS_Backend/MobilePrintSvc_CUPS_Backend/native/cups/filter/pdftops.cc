/*
 * "$Id: pdftops.c 9793 2011-05-20 03:49:49Z mike $"
 *
 *   PDF to PostScript filter front-end for CUPS.
 *
 *   Copyright 2007-2011 by Apple Inc.
 *   Copyright 1997-2006 by Easy Software Products.
 *
 *   These coded instructions, statements, and computer programs are the
 *   property of Apple Inc. and are protected by Federal copyright
 *   law.  Distribution and use rights are outlined in the file "LICENSE.txt"
 *   which should have been included with this file.  If this file is
 *   file is missing or damaged, see the license at "http://www.cups.org/".
 *
 * Contents:
 *
 *   main()       - Main entry for filter...
 *   cancel_job() - Flag the job as canceled.
 */

/*
 * Include necessary headers...
 */
#include<stdio.h>
#include<stdlib.h>
#include <cups/cups.h>
#include <cups/ppd.h>
#include <cups/string-private.h>
#include <cups/language-private.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <jni.h>
#include "popplertops.h"
//#include "pstops.h"
#include <android/log.h>
#include <JNIHelp.h>
//#include <android_runtime/AndroidRuntime.h>
#define HAVE_PDFTOPS 1
/*
 * Local functions...
 */

static void cancel_job(int sig);
int pdftops_main(int  argc,char *argv[]); 
extern "C" int pstops_main(int argc,char *argv[]);
 

/*
 * Local globals...
 */

static int		job_canceled = 0;

#define LOG_TAG "GenericPrintService"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)


//jin mple.mobilepdfprint

	
jint Java_com_siso_app_genericprintservice_GPrintPdfToPs_InvokePopplerPDF(JNIEnv* env, jobject thiz,jobjectArray stringArray){

	char *argv[100];
	int stringCount = env->GetArrayLength(stringArray);
	int i,count,lpout=0;

	for (i=0; i<stringCount; i++) {
        jstring string = (jstring) env->GetObjectArrayElement(stringArray, i);
		const char *arg = env->GetStringUTFChars(string, 0);
		argv[i] = const_cast<char *> (arg);
	}

	lpout = pdftops_main(stringCount,argv);
	return lpout;


}

static JNINativeMethod gsMethods[] = {
     /* name, signature, funcPtr */

        {"InvokePopplerPDF", "([Ljava/lang/String;)I", (void *)Java_com_siso_app_genericprintservice_GPrintPdfToPs_InvokePopplerPDF}
};



int register_android_jni_cups(JNIEnv* env)
{		
        jniRegisterNativeMethods(env, "com/siso/app/genericprintservice/GPrintPdfToPs", gsMethods, NELEM(gsMethods));

    return 0;
}


extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGI("JNI Error : GetEnv failed!");
        return result;
    }
    
    register_android_jni_cups(env);

    return JNI_VERSION_1_4;
}


/*
 * 'main()' - Main entry for filter...
 */

int					/* O - Exit status */
pdftops_main(int  argc,				/* I - Number of command-line args */
     char *argv[])			/* I - Command-line arguments */
{
  int		fd;			/* Copy file descriptor */
  char		*filename,		/* PDF file to convert */
		tempfile[1024];		/* Temporary file */
  char		buffer[8192];		/* Copy buffer */
  int		bytes;			/* Bytes copied */
  int		num_options;		/* Number of options */
  cups_option_t	*options;		/* Options */
  const char	*val;			/* Option value */
  int		orientation,		/* Output orientation */
		fit;			/* Fit output to default page size? */
  ppd_file_t	*ppd;			/* PPD file */
  ppd_size_t	*size;			/* Current page size */
  int		pdf_pid,		/* Process ID for pdftops */
		pdf_argc,		/* Number of args for pdftops */
		pstops_argc,		/*Number of args for pstops */
		pstops_pid,		/* Process ID of pstops filter */
		pstops_pipe[2],		/* Pipe to pstops filter */
		wait_children,		/* Number of child processes left */
		wait_pid,		/* Process ID from wait() */
		wait_status,		/* Status from child */
		exit_status = 0;	/* Exit status */
  char		*pdf_argv[100],		/* Arguments for pdftops/gs */
		pdf_width[255],		/* Paper width */
		pdf_height[255],	/* Paper height */
		pstops_path[1024],	/* Path to pstops program */
		*pstops_argv[8],	/* Arguments for pstops filter */
		*pstops_options,	/* Options for pstops filter */
		*pstops_start,		/* Start of pstops filter option */
		*pstops_end;		/* End of pstops filter option */
  const char	*cups_serverbin;	/* CUPS_SERVERBIN environment variable */
 char *ppdFilePath;
 char *psFilePath;

 

#if defined(HAVE_SIGACTION) && !defined(HAVE_SIGSET)
  struct sigaction action;		/* Actions for POSIX signals */
#endif /* HAVE_SIGACTION && !HAVE_SIGSET */


 /*
  * Make sure status messages are not buffered...
  */
	int ii=0;
	for(;ii<argc;ii++)
		LOGI("backend : pdftops_main : argv=%s",argv[ii]);	

  setbuf(stderr, NULL);

 /*
  * Ignore broken pipe signals...
  */

  signal(SIGPIPE, SIG_IGN);

 /*
  * Make sure we have the right number of arguments for CUPS!
  */

//  if (argc < 6 || argc > 7)
//  {
//    _cupsLangPrintf(stderr,
//                    _("Usage: %s job user title copies options [filename]"),
//                    argv[0]);
//    return (1);
//  }

 /*
  * Register a signal handler to cleanly cancel a job.
  */

#ifdef HAVE_SIGSET /* Use System V signals over POSIX to avoid bugs */
  sigset(SIGTERM, cancel_job);
#elif defined(HAVE_SIGACTION)
  memset(&action, 0, sizeof(action));

  sigemptyset(&action.sa_mask);
  action.sa_handler = cancel_job;
  sigaction(SIGTERM, &action, NULL);
#else
  signal(SIGTERM, cancel_job);
#endif /* HAVE_SIGSET */

 
   ppdFilePath = argv[6];
   psFilePath = argv[7];
   filename = argv[8];

 /*
  * Load the PPD file and mark options...
  */

  ppd = ppdOpenFile(ppdFilePath);

  num_options = cupsParseOptions(argv[5], 0, &options);

  ppdMarkDefaults(ppd);
  cupsMarkOptions(ppd, num_options, options);

 /*
  * Build the pstops command-line...
  */

  if ((cups_serverbin = getenv("CUPS_SERVERBIN")) == NULL)
    cups_serverbin = CUPS_SERVERBIN;

  snprintf(pstops_path, sizeof(pstops_path), "%s/pstops",cups_serverbin); //changed@chandrashekar

  pstops_options = strdup(argv[5]);
  
  if ((pstops_start = strstr(pstops_options, "fitplot")) != NULL &&
      (!pstops_start[7] || isspace(pstops_start[7] & 255)))
  {
   /*
    * Strip [no]fitplot option...
    */

    pstops_end = pstops_start + 7;

    if ((pstops_start - pstops_options) >= 2 &&
        !strncmp(pstops_start - 2, "no", 2))
      pstops_start -= 2;

    while (*pstops_end && isspace(*pstops_end & 255))
      pstops_end ++;

    _cups_strcpy(pstops_start, pstops_end);
  }

  if ((pstops_start = strstr(pstops_options, "fit-to-page")) != NULL &&
      (!pstops_start[11] || isspace(pstops_start[11] & 255)))
  {
   /*
    * Strip [no]fit-to-page option...
    */

    pstops_end = pstops_start + 11;

    if ((pstops_start - pstops_options) >= 2 &&
        !strncmp(pstops_start - 2, "no", 2))
      pstops_start -= 2;

    while (*pstops_end && isspace(*pstops_end & 255))
      pstops_end ++;

    _cups_strcpy(pstops_start, pstops_end);
  }

  if ((pstops_start = strstr(pstops_options, "landscape")) != NULL &&
      (!pstops_start[9] || isspace(pstops_start[9] & 255)))
  {
   /*
    * Strip [no]landscape option...
    */

    pstops_end = pstops_start + 9;

    if ((pstops_start - pstops_options) >= 2 &&
        !strncmp(pstops_start - 2, "no", 2))
      pstops_start -= 2;

    while (*pstops_end && isspace(*pstops_end & 255))
      pstops_end ++;

    _cups_strcpy(pstops_start, pstops_end);
  }

  if ((pstops_start = strstr(pstops_options, "orientation-requested=")) != NULL)
  {
   /*
    * Strip [no]fitplot option...
    */

    pstops_end = pstops_start + 22;
    while (*pstops_end && !isspace(*pstops_end & 255))
      pstops_end ++;

    _cups_strcpy(pstops_start, pstops_end);
  }

  pstops_argv[0] = argv[0];		/* Printer */
  pstops_argv[1] = argv[1];		/* Job */
  pstops_argv[2] = argv[2];		/* User */
  pstops_argv[3] = argv[3];		/* Title */
  pstops_argv[4] = argv[4];		/* Copies */
  pstops_argv[5] = pstops_options;	/* Options */
  pstops_argv[6] = ppdFilePath;
  pstops_argv[7] = psFilePath;
  pstops_argc = 8;
 /*
  * Build the command-line for the pdftops or gs filter...
  */

#ifdef HAVE_PDFTOPS
  pdf_argv[0] = (char *)"popplerpdf"; //changed@chandrashekar
  pdf_argc    = 1;
#else
  pdf_argv[0] = (char *)"gs";
  pdf_argv[1] = (char *)"-q";
  pdf_argv[2] = (char *)"-dNOPAUSE";
  pdf_argv[3] = (char *)"-dBATCH";
  pdf_argv[4] = (char *)"-dSAFER";
#  ifdef HAVE_GHOSTSCRIPT_PS2WRITE
  pdf_argv[5] = (char *)"-sDEVICE=ps2write";
#  else
  pdf_argv[5] = (char *)"-sDEVICE=pswrite";
#  endif /* HAVE_GHOSTSCRIPT_PS2WRITE */
  pdf_argv[6] = (char *)"-sOUTPUTFILE=%stdout";
  pdf_argc    = 7;
#endif /* HAVE_PDFTOPS */

  if (ppd)
  {
   /*
    * Set language level and TrueType font handling...
    */

    if (ppd->language_level == 1)
    {
#ifdef HAVE_PDFTOPS
      pdf_argv[pdf_argc++] = (char *)"-level1";
      pdf_argv[pdf_argc++] = (char *)"-noembtt";
#else
      pdf_argv[pdf_argc++] = (char *)"-dLanguageLevel=1";
#endif /* HAVE_PDFTOPS */
    }
    else if (ppd->language_level == 2)
    {
#ifdef HAVE_PDFTOPS
      pdf_argv[pdf_argc++] = (char *)"-level2";
      if (!ppd->ttrasterizer)
	pdf_argv[pdf_argc++] = (char *)"-noembtt";
#else
      pdf_argv[pdf_argc++] = (char *)"-dLanguageLevel=2";
#endif /* HAVE_PDFTOPS */
    }
    else
#ifdef HAVE_PDFTOPS
      pdf_argv[pdf_argc++] = (char *)"-level3";
#else
      pdf_argv[pdf_argc++] = (char *)"-dLanguageLevel=3";
#endif /* HAVE_PDFTOPS */

    if ((val = cupsGetOption("fitplot", num_options, options)) == NULL)
      val = cupsGetOption("fit-to-page", num_options, options);

    if (val && _cups_strcasecmp(val, "no") && _cups_strcasecmp(val, "off") &&
	_cups_strcasecmp(val, "false"))
      fit = 1;
    else
      fit = 0;

   /*
    * Set output page size...
    */

    size = ppdPageSize(ppd, NULL);
    if (size && fit)
    {
     /*
      * Got the size, now get the orientation...
      */

      orientation = 0;

      if ((val = cupsGetOption("landscape", num_options, options)) != NULL)
      {
	if (_cups_strcasecmp(val, "no") != 0 && _cups_strcasecmp(val, "off") != 0 &&
	    _cups_strcasecmp(val, "false") != 0)
	  orientation = 1;
      }
      else if ((val = cupsGetOption("orientation-requested", num_options,
                                    options)) != NULL)
      {
       /*
	* Map IPP orientation values to 0 to 3:
	*
	*   3 = 0 degrees   = 0
	*   4 = 90 degrees  = 1
	*   5 = -90 degrees = 3
	*   6 = 180 degrees = 2
	*/

	orientation = atoi(val) - 3;
	if (orientation >= 2)
	  orientation ^= 1;
      }

#ifdef HAVE_PDFTOPS
      if (orientation & 1)
      {
	snprintf(pdf_width, sizeof(pdf_width), "%.0f", size->length);
	snprintf(pdf_height, sizeof(pdf_height), "%.0f", size->width);
      }
      else
      {
	snprintf(pdf_width, sizeof(pdf_width), "%.0f", size->width);
	snprintf(pdf_height, sizeof(pdf_height), "%.0f", size->length);
      }

      pdf_argv[pdf_argc++] = (char *)"-paperw";
      pdf_argv[pdf_argc++] = pdf_width;
      pdf_argv[pdf_argc++] = (char *)"-paperh";
      pdf_argv[pdf_argc++] = pdf_height;
      pdf_argv[pdf_argc++] = (char *)"-expand";

#else
      if (orientation & 1)
      {
	snprintf(pdf_width, sizeof(pdf_width), "-dDEVICEWIDTHPOINTS=%.0f",
	         size->length);
	snprintf(pdf_height, sizeof(pdf_height), "-dDEVICEHEIGHTPOINTS=%.0f",
	         size->width);
      }
      else
      {
	snprintf(pdf_width, sizeof(pdf_width), "-dDEVICEWIDTHPOINTS=%.0f",
	         size->width);
	snprintf(pdf_height, sizeof(pdf_height), "-dDEVICEHEIGHTPOINTS=%.0f",
	         size->length);
      }

      pdf_argv[pdf_argc++] = pdf_width;
      pdf_argv[pdf_argc++] = pdf_height;
#endif /* HAVE_PDFTOPS */
    }
#if defined(HAVE_PDFTOPS) && defined(HAVE_PDFTOPS_WITH_ORIGPAGESIZES)
    else
    {
     /*
      *  Use the page sizes of the original PDF document, this way documents
      *  which contain pages of different sizes can be printed correctly
      */

      pdf_argv[pdf_argc++] = (char *)"-origpagesizes";
    }
#endif /* HAVE_PDFTOPS && HAVE_PDFTOPS_WITH_ORIGPAGESIZES */
  }

#ifdef HAVE_PDFTOPS
  pdf_argv[pdf_argc++] = filename;
  pdf_argv[pdf_argc++] = (char *)"-";
#else
  pdf_argv[pdf_argc++] = (char *)"-c";
  pdf_argv[pdf_argc++] = (char *)"save pop";
  pdf_argv[pdf_argc++] = (char *)"-f";
  pdf_argv[pdf_argc++] = filename;
#endif /* HAVE_PDFTOPS */

  pdf_argv[pdf_argc] = NULL;

 /*
  * Execute "pdftops/gs | pstops"...
  */

  if (pipe(pstops_pipe))
  {
    perror("DEBUG: Unable to create pipe");

    exit_status = 1;
    goto error;
  }

  if ((pdf_pid = fork()) == 0)
  {
   /*
    * Child comes here...
    */
    dup2(pstops_pipe[1], 1);
    close(pstops_pipe[0]);
    close(pstops_pipe[1]);

#ifdef HAVE_PDFTOPS
int ret;
    	
    ret = popplerpdf_main(pdf_argc, pdf_argv);
    LOGI("backend/pdftopd_main/popplerpdf_main returned with value =%d",ret);    
  
  if(ret == 0)
	exit(0);
   else
	exit(1); 
#else
    execv(CUPS_GHOSTSCRIPT, pdf_argv);
    perror("DEBUG: Unable to execute gs program");
#endif /* HAVE_PDFTOPS */

    exit(1);
  }
  else if (pdf_pid < 0)
  {
   /*
    * Unable to fork!
    */

#ifdef HAVE_PDFTOPS
    LOGI("Error: unable to execute popplerpdf_main");
    perror("DEBUG: Unable to execute popplerpdf program"); //changed@chandrashekar
#else
    perror("DEBUG: Unable to execute gs program");
#endif /* HAVE_PDFTOPS */

    exit_status = 1;
    goto error;
  }

  fprintf(stderr, "DEBUG: Started filter %s (PID %d)\n", pdf_argv[0], pdf_pid);

  if ((pstops_pid = fork()) == 0)
  {
   /*
    * Child comes here...
    */
 	
    dup2(pstops_pipe[0], 0);
    close(pstops_pipe[0]);
    close(pstops_pipe[1]);
      
    int ret1 = pstops_main(pstops_argc,pstops_argv);
    LOGI("pstops_main finished with return value=%d",ret1);
    if(ret1 == 0)
		exit(0);
    else
		exit(1);
   
  }
  else if (pstops_pid < 0)
  {
   /*
    * Unable to fork!
    */

    perror("DEBUG: Unable to execute pstops program");

    exit_status = 1;
    goto error;
  }

  fprintf(stderr, "DEBUG: Started filter pstops (PID %d)\n", pstops_pid);

  close(pstops_pipe[0]);
  close(pstops_pipe[1]);

 /*
  * Wait for the child processes to exit...
  */

  wait_children = 2;

  while (wait_children > 0)
  {
   /*
    * Wait until we get a valid process ID or the job is canceled...
    */

    while ((wait_pid = wait(&wait_status)) < 0 && errno == EINTR)
    {
      if (job_canceled)
      {
	kill(pdf_pid, SIGTERM);
	kill(pstops_pid, SIGTERM);

	job_canceled = 0;
      }
    }

    if (wait_pid < 0)
      break;

    wait_children --;

   /*
    * Report child status...
    */

    if (wait_status)
    {
      if (WIFEXITED(wait_status))
      {
	exit_status = WEXITSTATUS(wait_status);

        fprintf(stderr, "DEBUG: PID %d (%s) stopped with status %d!\n",
	        wait_pid,
#ifdef HAVE_PDFTOPS
                wait_pid == pdf_pid ? "popplerpdf" : "pstops", //changed@chandrashekar
#else
                wait_pid == pdf_pid ? "gs" : "pstops",
#endif /* HAVE_PDFTOPS */
		exit_status);
      }
      else if (WTERMSIG(wait_status) == SIGTERM)
      {
        fprintf(stderr,
	        "DEBUG: PID %d (%s) was terminated normally with signal %d!\n",
	        wait_pid,
#ifdef HAVE_PDFTOPS
                wait_pid == pdf_pid ? "popplerpdf" : "pstops", //changed@chandrashekar
#else
                wait_pid == pdf_pid ? "gs" : "pstops",
#endif /* HAVE_PDFTOPS */
		exit_status);
      }
      else
      {
	exit_status = WTERMSIG(wait_status);

        fprintf(stderr, "DEBUG: PID %d (%s) crashed on signal %d!\n", wait_pid,
#ifdef HAVE_PDFTOPS
                wait_pid == pdf_pid ? "popplerpdf" : "pstops", //changed@chandrashekar
#else
                wait_pid == pdf_pid ? "gs" : "pstops",
#endif /* HAVE_PDFTOPS */
		exit_status);
      }
    }
    else
    {
      fprintf(stderr, "DEBUG: PID %d (%s) exited with no errors.\n", wait_pid,
#ifdef HAVE_PDFTOPS
	      wait_pid == pdf_pid ? "popplerpdf" : "pstops"); //changed@chandrashekar
#else
	      wait_pid == pdf_pid ? "gs" : "pstops");
#endif /* HAVE_PDFTOPS */
    }
  }

 /*
  * Cleanup and exit...
  */

  error:

  if (tempfile[0])
    unlink(tempfile);

  return (exit_status);
}


/*
 * 'cancel_job()' - Flag the job as canceled.
 */

static void
cancel_job(int sig)			/* I - Signal number (unused) */
{
  (void)sig;

  job_canceled = 1;
}


/*
 * End of "$Id: pdftops.c 9793 2011-05-20 03:49:49Z mike $".
 */
