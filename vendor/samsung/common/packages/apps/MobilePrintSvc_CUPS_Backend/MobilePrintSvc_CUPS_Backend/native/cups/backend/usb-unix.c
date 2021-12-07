/*
 * "$Id: usb-unix.c 9793 2011-05-20 03:49:49Z mike $"
 *
 *   USB port backend for CUPS.
 *
 *   This file is included from "usb.c" when compiled on UNIX/Linux.
 *
 *   Copyright 2007-2011 by Apple Inc.
 *   Copyright 1997-2007 by Easy Software Products, all rights reserved.
 *
 *   These coded instructions, statements, and computer programs are the
 *   property of Apple Inc. and are protected by Federal copyright
 *   law.  Distribution and use rights are outlined in the file "LICENSE.txt"
 *   "LICENSE" which should have been included with this file.  If this
 *   file is missing or damaged, see the license at "http://www.cups.org/".
 *
 *   This file is subject to the Apple OS-Developed Software exception.
 *
 * Contents:
 *
 *   print_device() - Print a file to a USB device.
 *   list_devices() - List all USB devices.
 *   open_device()  - Open a USB device...
 *   side_cb()      - Handle side-channel requests...
 */

/*
 * Include necessary headers.
 */

#include <sys/select.h>
#include "backend-private.h"

#ifdef WIN32
#  include <io.h>
#else
#  include <unistd.h>
#  include <fcntl.h>
#  include <termios.h>
#endif /* WIN32 */


#include<android/log.h>
#include "usb-unix.h"
#include <string.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

#include<jni.h>

#define __linux


#define LOG_TAG "GenericPrintService"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
/*
 * Local functions...
 */

//void  list_devices(void);
//int   print_device(const char *uri, const char *hostname,const char *resource, char *options,int print_fd, int copies, int argc, char *argv[]);


/*
 * Include the vendor-specific USB implementation...
 */

/*#ifdef HAVE_USB_H
#  include "usb-libusb.c"
#elif defined(__APPLE__)
#  include "usb-darwin.c"
#elif defined(__linux) || defined(__sun) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
#  include "usb-unix.c"
#else
#endif*/

/*
 * Local functions...
 */
static char	t_device_id[1024];	/* Device ID string */
static char	t_device_uri[1024];		/* Device URI string */
static char	t_make_model[1024];		/* Make and model */
static Usb_Device_Info device_info = {NULL,NULL,NULL};

static int	open_device(const char *uri, int *use_bc);
static int	side_cb(int print_fd, int device_fd, int snmp_fd,
		        http_addr_t *addr, int use_bc);


typedef void (*update_state)(int,unsigned int); 
update_state update_job_state;				


				
				
				
/*
 * 'print_device()' - Print a file to a USB device.
 */

int					/* O - Exit status */
print_device(const char *uri,		/* I - Device URI */
             const char *hostname,	/* I - Hostname/manufacturer */
             const char *resource,	/* I - Resource/modelname */
	     char       *options,	/* I - Device options/serial number */
	     int        print_fd,	/* I - File descriptor to print */
	     int        copies,		/* I - Copies to print */
	     int	argc,		/* I - Number of command-line arguments (6 or 7) */
	     char	*argv[])	/* I - Command-line arguments */
{
  int		use_bc;			/* Use backchannel path? */
  int		device_fd;		/* USB device */
  ssize_t	tbytes;			/* Total number of bytes written */
  struct termios opts;			/* Parallel port options */

   
  (void)argc;
  (void)argv;

 /*
  * Open the USB port device...
  */
   int wait_count=0;  

  do
  {
//#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
   /*
    * *BSD's ulpt driver currently does not support the
    * back-channel, incorrectly returns data ready on a select(),
    * and locks up on read()...
    */

//    use_bc = 0;

//#elif defined(__sun)
   /*
    * CUPS STR #3028: Solaris' usbprn driver apparently does not support
    * select() or poll(), so we can't support backchannel...
    */

//    use_bc = 0;

//#else
   /*
    * Disable backchannel data when printing to Brother, Canon, or
    * Minolta USB printers - apparently these printers will return
    * the IEEE-1284 device ID over and over and over when they get
    * a read request...
    */

    use_bc = _cups_strcasecmp(hostname, "Brother") &&
             _cups_strcasecmp(hostname, "Canon") &&
             _cups_strncasecmp(hostname, "Konica", 6) &&
             _cups_strncasecmp(hostname, "Minolta", 7);
//#endif /* __FreeBSD__ || __NetBSD__ || __OpenBSD__ || __DragonFly__ */
    
    if ((device_fd = open_device(uri, &use_bc)) == -1)
    {
	
      if (getenv("CLASS") != NULL)
      {
       /*
        * If the CLASS environment variable is set, the job was submitted
	* to a class and not to a specific queue.  In this case, we want
	* to abort immediately so that the job can be requeued on the next
	* available printer in the class.
	*/

        _cupsLangPrintFilter(stderr, "INFO",
			     _("Unable to contact printer, queuing on next "
			       "printer in class."));
        LOGI("USB:print_device: unable to connect printer wait");
       /*
        * Sleep 5 seconds to keep the job from requeuing too rapidly...
	*/

	sleep(5);

        return (CUPS_BACKEND_FAILED);
      }

      if (errno == EBUSY)
      {
        _cupsLangPrintFilter(stderr, "INFO",
			     _("Printer busy, will retry in 10 seconds."));
		LOGI(" >>> print_device: Printer busy, will retry in 10 seconds...\n");
	     sleep(10);
      }
      else if (errno == ENXIO || errno == EIO || errno == ENOENT ||
               errno == ENODEV)
      {
        _cupsLangPrintFilter(stderr, "INFO",
			     _("Printer not connected, will retry in 30 "
			       "seconds."));
		LOGI(" >>> print_device: Printer not connected, will retry in 15 seconds");
	wait_count++;
	if(wait_count > 8)
		return(CUPS_BACKEND_FAILED);
	sleep(15);
      }
      else
      {
	_cupsLangPrintError("ERROR", _("Unable to open device file"));
	LOGI(" >>> print_device: Unable to open device file...\n");
	return (CUPS_BACKEND_FAILED);
      }
    }
  }
  while (device_fd < 0);

  fputs("STATE: -connecting-to-device\n", stderr);
  

 /*
  * Set any options provided...
  */
  
  tcgetattr(device_fd, &opts);
  
  opts.c_lflag &= ~(ICANON | ECHO | ISIG);	/* Raw mode */
  
  /**** No options supported yet ****/
  
  tcsetattr(device_fd, TCSANOW, &opts);
  
 /*
  * Finally, send the print file...
  */

  tbytes = 0;

  while (copies > 0 && tbytes >= 0)
  {
    copies --;

    if (print_fd != 0)
    {
	
      fputs("PAGE: 1 1\n", stderr);
	  LOGI(" >>> print_device: PAGE: 1 1\n");
      lseek(print_fd, 0, SEEK_SET);
    }

//#ifdef __sun
   /*
    * CUPS STR #3028: Solaris' usbprn driver apparently does not support
    * select() or poll(), so we can't support the sidechannel either...
    */

//    tbytes = backendRunLoop(print_fd, device_fd, -1, NULL, use_bc, 1, NULL);

//#else
   
    tbytes = backendRunLoop(print_fd, device_fd, -1, NULL, use_bc, 1, side_cb);
//#endif /* __sun */

    if (print_fd != 0 && tbytes >= 0){
      _cupsLangPrintFilter(stderr, "INFO", _("Print file sent."));
	  
    }
  }

 /*
  * Close the USB port and return...
  */

  close(device_fd);

  return (CUPS_BACKEND_OK);
}

/*
 * 'getDevFile_devices()' - List all USB devices.
 */

Usb_Device_Info * 
getDevFile_devices() 
{
  int	fd;				/* File descriptor */
  char	device[255],			/* Device filename */
	device_id[1024],		/* Device ID string */
	device_uri[1024],		/* Device URI string */
	make_model[1024];		/* Make and model */

 
    sprintf(device, "/dev/usb/lp%d", 0);
   
    if ((fd = open(device, O_RDWR | O_EXCL)) < 0)
    {
		LOGI("Error : getDevFile_devices errno: %d\n", errno);
	  return &device_info;
      
    }

	
    if (!backendGetDeviceID(fd, device_id, sizeof(device_id),
                            make_model, sizeof(make_model),
			    "usb", device_uri, sizeof(device_uri)))
      cupsBackendReport("direct", device_uri, make_model, make_model,
                        device_id, NULL);
						
						
	  LOGI("getDevFile_devices device_uri: %s\n", device_uri);
	  LOGI("getDevFile_devices make_model: %s\n", make_model);
	  LOGI("getDevFile_devices device_id: %s\n", device_id);


	 device_info.device_uri = &t_device_uri;
	 memset(device_info.device_uri,0,sizeof(device_info.device_uri));
   	 strcpy(device_info.device_uri,device_uri);

	 device_info.make_model = &t_make_model;
	 memset(device_info.make_model,0,sizeof(device_info.make_model));
   	 strcpy(device_info.make_model,make_model);

	 device_info.device_id = &t_device_id;
	 memset(device_info.device_id,0,sizeof(device_info.device_id));
   	 strcpy(device_info.device_id,device_id);
  
 	
    close(fd);
	return &device_info;

}

 

/*
 * 'list_devices()' - List all USB devices.
 */

void
list_devices(void)
{
//#ifdef __linux
  int	i;				/* Looping var */
  int	fd;				/* File descriptor */
  char	device[255],			/* Device filename */
	device_id[1024],		/* Device ID string */
	device_uri[1024],		/* Device URI string */
	make_model[1024];		/* Make and model */

   
 /*
  * Try to open each USB device...
  */

  for (i = 0; i < 16; i++)
  { 
   //printf("searching usb devices\n");
   /*
    * Linux has a long history of changing the standard filenames used
    * for USB printer devices.  We get the honor of trying them all...
    */

    sprintf(device, "/dev/usb/lp%d", i);
   
    if ((fd = open(device, O_RDWR | O_EXCL)) < 0)
    {
	  
	  LOGI("list_devices errno: %d\n", errno);
      
	  //if (errno != ENOENT)
	  continue;

      //sprintf(device, "/dev/usb/lp%d", i);
      //printf("list_devices device: %s\n", device);
      //if ((fd = open(device, O_RDWR | O_EXCL)) < 0)
      //{
	  // printf("list_devices errno: %d\n", errno);
	  //if (errno != ENOENT)
	  //continue;

	  //sprintf(device, "/dev/usb/usblp%d", i);

      //	if ((fd = open(device, O_RDWR | O_EXCL)) < 0)
	  //continue;
      //}
    }

	
    if (!backendGetDeviceID(fd, device_id, sizeof(device_id),
                            make_model, sizeof(make_model),
			    "usb", device_uri, sizeof(device_uri)))
      cupsBackendReport("direct", device_uri, make_model, make_model,
                        device_id, NULL);
						
						
	  LOGI("list_devices device_uri: %s\n", device_uri);
	  LOGI("list_devices make_model: %s\n", make_model);
	  LOGI("list_devices device_id: %s\n", device_id);


    close(fd);
  }
//#elif defined(__sgi)
//#elif defined(__sun) && defined(ECPPIOC_GETDEVID)
#if 0
  int	i;			/* Looping var */
  int	fd;			/* File descriptor */
  char	device[255],		/* Device filename */
	device_id[1024],	/* Device ID string */
	device_uri[1024],	/* Device URI string */
	make_model[1024];	/* Make and model */


 /*
  * Open each USB device...
  */

  for (i = 0; i < 8; i ++)
  {
    sprintf(device, "/dev/usb/printer%d", i);

    if ((fd = open(device, O_WRONLY | O_EXCL)) >= 0)
    {
      if (!backendGetDeviceID(fd, device_id, sizeof(device_id),
                              make_model, sizeof(make_model),
			      "usb", device_uri, sizeof(device_uri)))
	cupsBackendReport("direct", device_uri, make_model, make_model,
			  device_id, NULL);

      close(fd);
    }
  }
//#elif defined(__hpux)
//#elif defined(__osf)
//#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
  int   i;                      /* Looping var */
  char  device[255];            /* Device filename */


  for (i = 0; i < 8; i ++)
  {
    sprintf(device, "/dev/ulpt%d", i);
    if (!access(device, 0))
      printf("direct usb:%s \"Unknown\" \"USB Printer #%d\"\n", device, i + 1);

    sprintf(device, "/dev/unlpt%d", i);
    if (!access(device, 0))
      printf("direct usb:%s \"Unknown\" \"USB Printer #%d (no reset)\"\n", device, i + 1);
  }
  #endif
//#endif
}


/*
 * 'open_device()' - Open a USB device...
 */

static int				/* O - File descriptor or -1 on error */
open_device(const char *uri,		/* I - Device URI */
            int        *use_bc)		/* O - Set to 0 for unidirectional */
{
  int	fd;				/* File descriptor */


 /*
  * The generic implementation just treats the URI as a device filename...
  * Specific operating systems may also support using the device serial
  * number and/or make/model.
  */

  if (!strncmp(uri, "usb:/dev/", 9))
#ifdef __linux
  {
   /*
    * Do not allow direct devices anymore...
    */

    errno = ENODEV;
    return (-1);
  }
  else if (!strncmp(uri, "usb://", 6))
  {
   /*
    * For Linux, try looking up the device serial number or model...
    */

    int		i;			/* Looping var */
    int		busy;			/* Are any ports busy? */
    char	device[255],		/* Device filename */
		device_id[1024],	/* Device ID string */
		make_model[1024],	/* Make and model */
		device_uri[1024];	/* Device URI string */


   /*
    * Find the correct USB device...
    */

    for (;;)
    {
      for (busy = 0, i = 0; i < 16; i ++)
      {
       /*
	* Linux has a long history of changing the standard filenames used
	* for USB printer devices.  We get the honor of trying them all...
	*/

	sprintf(device, "/dev/usblp%d", i);

	if ((fd = open(device, O_RDWR | O_EXCL)) < 0 && errno == ENOENT)
	{
	  sprintf(device, "/dev/usb/lp%d", i);

	  if ((fd = open(device, O_RDWR | O_EXCL)) < 0 && errno == ENOENT)
	  {
		LOGI("USB:open_device: No /dev/usb/lp%d no file",i);
		return(-1);
	    sprintf(device, "/dev/usb/usblp%d", i);
		
    	    if ((fd = open(device, O_RDWR | O_EXCL)) < 0 && errno == ENOENT)
	      continue;
	  }
	}
        
	if (fd >= 0)
	{
	  backendGetDeviceID(fd, device_id, sizeof(device_id),
                             make_model, sizeof(make_model),
			     "usb", device_uri, sizeof(device_uri));
	}
	else
	{
	 /*
	  * If the open failed because it was busy, flag it so we retry
	  * as needed...
	  */
	LOGI("USB:open_device : open fail retry");
	
	  if (errno == EBUSY)
	    busy = 1;

	  device_uri[0] = '\0';
        }

        if (!strcmp(uri, device_uri))
	{
	 /*
	  * Yes, return this file descriptor...
	  */

	  fprintf(stderr, "DEBUG: Printer using device file \"%s\"...\n",
		  device);

	  return (fd);
	}

       /*
	* This wasn't the one...
	*/
	LOGI("This was not correct printer");
        if (fd >= 0)
	  close(fd);
      }

     /*
      * If we get here and at least one of the printer ports showed up
      * as "busy", then sleep for a bit and retry...
      */

      if (busy)
	_cupsLangPrintFilter(stderr, "INFO",
			     _("Printer is busy, will retry in 5 seconds."));

      sleep(5);
    }
  }
#elif defined(__sun) && defined(ECPPIOC_GETDEVID)
  {
   /*
    * Do not allow direct devices anymore...
    */

    errno = ENODEV;
    return (-1);
  }
  else if (!strncmp(uri, "usb://", 6))
  {
   /*
    * For Solaris, try looking up the device serial number or model...
    */

    int		i;			/* Looping var */
    int		busy;			/* Are any ports busy? */
    char	device[255],		/* Device filename */
		device_id[1024],	/* Device ID string */
		make_model[1024],	/* Make and model */
		device_uri[1024];	/* Device URI string */


   /*
    * Find the correct USB device...
    */

    do
    {
      for (i = 0, busy = 0; i < 8; i ++)
      {
	sprintf(device, "/dev/usb/printer%d", i);

	if ((fd = open(device, O_WRONLY | O_EXCL)) >= 0)
	  backendGetDeviceID(fd, device_id, sizeof(device_id),
                             make_model, sizeof(make_model),
			     "usb", device_uri, sizeof(device_uri));
	else
	{
	 /*
	  * If the open failed because it was busy, flag it so we retry
	  * as needed...
	  */

	  if (errno == EBUSY)
	    busy = 1;

	  device_uri[0] = '\0';
        }

        if (!strcmp(uri, device_uri))
	{
	 /*
	  * Yes, return this file descriptor...
	  */

          fputs("DEBUG: Setting use_bc to 0!\n", stderr);

          *use_bc = 0;

	  return (fd);
	}

       /*
	* This wasn't the one...
	*/

        if (fd >= 0)
	  close(fd);
      }

     /*
      * If we get here and at least one of the printer ports showed up
      * as "busy", then sleep for a bit and retry...
      */

      if (busy)
      {
	_cupsLangPrintFilter(stderr, "INFO",
			     _("Printer is busy, will retry in 5 seconds."));
	sleep(5);
      }
    }
    while (busy);

   /*
    * Couldn't find the printer, return "no such device or address"...
    */

    errno = ENODEV;

    return (-1);
  }
#else
  {
    if (*use_bc)
      fd = open(uri + 4, O_RDWR | O_EXCL);
    else
      fd = -1;

    if (fd < 0)
    {
      fd      = open(uri + 4, O_WRONLY | O_EXCL);
      *use_bc = 0;
    }

    return (fd);
  }
#endif /* __linux */
  else
  {
    errno = ENODEV;
    return (-1);
  }
}


/*
 * 'side_cb()' - Handle side-channel requests...
 */

static int				/* O - 0 on success, -1 on error */
side_cb(int         print_fd,		/* I - Print file */
        int         device_fd,		/* I - Device file */
        int         snmp_fd,		/* I - SNMP socket (unused) */
	http_addr_t *addr,		/* I - Device address (unused) */
	int         use_bc)		/* I - Using back-channel? */
{
  cups_sc_command_t	command;	/* Request command */
  cups_sc_status_t	status;		/* Request/response status */
  char			data[2048];	/* Request/response data */
  int			datalen;	/* Request/response data size */


  (void)snmp_fd;
  (void)addr;

  datalen = sizeof(data);

  if (cupsSideChannelRead(&command, &status, data, &datalen, 1.0))
    return (-1);

  switch (command)
  {
    case CUPS_SC_CMD_DRAIN_OUTPUT :
        if (backendDrainOutput(print_fd, device_fd))
	  status = CUPS_SC_STATUS_IO_ERROR;
	else if (ioctl(device_fd, TCSBRK,1)) //(tcdrain(device_fd)) changed@chandrashekar
	  status = CUPS_SC_STATUS_IO_ERROR;
	else
	  status = CUPS_SC_STATUS_OK;

	datalen = 0;
        break;

    case CUPS_SC_CMD_GET_BIDI :
	status  = CUPS_SC_STATUS_OK;
        data[0] = use_bc;
        datalen = 1;
        break;

    case CUPS_SC_CMD_GET_DEVICE_ID :
        memset(data, 0, sizeof(data));

        if (backendGetDeviceID(device_fd, data, sizeof(data) - 1,
	                       NULL, 0, NULL, NULL, 0))
        {
	  status  = CUPS_SC_STATUS_NOT_IMPLEMENTED;
	  datalen = 0;
	}
	else
        {
	  status  = CUPS_SC_STATUS_OK;
	  datalen = strlen(data);
	}
        break;

    default :
        status  = CUPS_SC_STATUS_NOT_IMPLEMENTED;
	datalen = 0;
	break;
  }

  return (cupsSideChannelWrite(command, status, data, datalen, 1.0));
}


/*
 * End of "$Id: usb-unix.c 9793 2011-05-20 03:49:49Z mike $".
 */

int                                     /* O - Exit status */
usbDevFile_main(int  argc,                         /* I - Number of command-line arguments (6 or 7) */
     char *argv[],update_state job_state_fun)                      /* I - Command-line arguments */
{
  int           print_fd;               /* Print file */
  int           copies;                 /* Number of copies to print */
  int           status;                 /* Exit status */
  int           port;                   /* Port number (not used) */
  const char    *uri;                   /* Device URI */
  char          method[255],            /* Method in URI */
                hostname[1024],         /* Hostname */
                username[255],          /* Username info (not used) */
                resource[1024],         /* Resource info (device and options) */
                *options;               /* Pointer to options */
#if defined(HAVE_SIGACTION) && !defined(HAVE_SIGSET)
  struct sigaction action;              /* Actions for POSIX signals */
#endif /* HAVE_SIGACTION && !HAVE_SIGSET */

	update_job_state = job_state_fun;
  int tem_cnt = 0;
  for(;tem_cnt<argc;tem_cnt++)
  	{
  		fprintf(stderr, "IPP: tem_cnt = %d argv = %s\n",tem_cnt,argv[tem_cnt]);
  	}
	
	//list_devices();
 /*
  * Make sure status messages are not buffered...
  */

  setbuf(stderr, NULL);

 /*
  * Ignore SIGPIPE signals...
  */

#ifdef HAVE_SIGSET
  sigset(SIGPIPE, SIG_IGN);
#elif defined(HAVE_SIGACTION)
  memset(&action, 0, sizeof(action));
  action.sa_handler = SIG_IGN;
  sigaction(SIGPIPE, &action, NULL);
#else
  signal(SIGPIPE, SIG_IGN);
#endif /* HAVE_SIGSET */

 /*
  * Check command-line...
  */

 if (argc == 1)
  {
    list_devices();
    return (CUPS_BACKEND_OK);
  }
  else if (argc < 6 || argc > 7)
  {
 _cupsLangPrintf(stderr,
                    _("Usage: %s job-id user title copies options [file]"),
                    argv[0]);
    return (CUPS_BACKEND_FAILED);
  }

 /*
  * Extract the device name and options from the URI...
  */

  uri = cupsBackendDeviceURI(argv);
 
  
  
  if (httpSeparateURI(HTTP_URI_CODING_ALL, uri,
                      method, sizeof(method), username, sizeof(username),
                      hostname, sizeof(hostname), &port,
                      resource, sizeof(resource)) < HTTP_URI_OK)
  {
		LOGI("USb-unix / ERROR: No device URI found ");
    _cupsLangPrintFilter(stderr, "ERROR",
                         _("No device URI found in argv[0] or in DEVICE_URI "
                           "environment variable."));
    return (1);
  }

  


 /*
  * See if there are any options...
  */

  if ((options = strchr(resource, '?')) != NULL)
  {
   /*
    * Yup, terminate the device name string and move to the first
    * character of the options...
    */

    *options++ = '\0';
  }

 /*
  * If we have 7 arguments, print the file named on the command-line.
  * Otherwise, send stdin instead...
  */

  if (argc == 6)
  {
    print_fd = 0;
    copies   = 1;
  }
  else
  {
   /*
    * Try to open the print file...
    */

 if ((print_fd = open(argv[6], O_RDONLY)) < 0)
    {
		LOGI("USb-unix / ERROR: Unable to open print file");
      _cupsLangPrintError("ERROR", _("Unable to open print file"));
      return (CUPS_BACKEND_FAILED);
    }

    copies = atoi(argv[4]);
  }

 /*
  * Finally, send the print file...
  */

  status = print_device(uri, hostname, resource, options, print_fd, copies,
                        argc, argv);

 /*
  * Close the input file and return...
  */

  if (print_fd != 0)
    close(print_fd);

  return (status);

}



jobjectArray Java_com_siso_app_genericprintservice_GPrintUsbManager_GetDevFileUsbDevice(JNIEnv *env, jobject thiz)
{
	 Usb_Device_Info *device_info =  getDevFile_devices();
	 jclass stringClazz = (*env)->FindClass(env,"java/lang/String");
	 jobjectArray stringArray = (*env)->NewObjectArray(env,3, stringClazz, NULL);

	 if(device_info->device_uri != NULL)
	 	{
		 jstring uri = (*env)->NewStringUTF(env,device_info->device_uri);
		 (*env)->SetObjectArrayElement(env,stringArray, 0, uri);

		 (*env)->DeleteLocalRef(env,uri);
		 //free((char *)device_info->device_uri);
	 	} 
     
	 if(device_info->make_model != NULL)
	 	{
		 jstring model = (*env)->NewStringUTF(env,device_info->make_model);
		 (*env)->SetObjectArrayElement(env,stringArray, 1, model);

		 (*env)->DeleteLocalRef(env,model);
		 //free((char *)device_info->make_model);
	 	} 

	if(device_info->device_id != NULL) 
	 	{
		 jstring device_id = (*env)->NewStringUTF(env,device_info->device_id);
		 (*env)->SetObjectArrayElement(env,stringArray, 2, device_id);

		 (*env)->DeleteLocalRef(env,device_id);
		 //free((char *)device_info->device_id);
	 	} 

	 
	 return stringArray;
}

static JNINativeMethod sSearchMethods[] = {
     // name, signature, funcPtr 
	{"GetDevFileUsbDevice", "()[Ljava/lang/String;",(void *) Java_com_siso_app_genericprintservice_GPrintUsbManager_GetDevFileUsbDevice}	
};


int register_android_jni_cups(JNIEnv* env)  
{	
	jclass clazz;
    clazz = (*env)->FindClass(env, "com/siso/app/genericprintservice/GPrintUsbManager");
    if (clazz == NULL) {
		return -1;
    }
    if ((*env)->RegisterNatives(env, clazz, sSearchMethods, 1) < 0) {
        return -1;
    }
    return 0;
	
    
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
   	JNIEnv* env;
    if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK)
        return -1;
		
    register_android_jni_cups(env);

    return JNI_VERSION_1_4;
}
