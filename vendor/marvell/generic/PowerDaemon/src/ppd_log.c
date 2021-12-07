/*
 *  Copyright (C) 2002-2006  Mattia Dongili <malattia@linux.it>
 *                           George Staikos <staikos@0wned.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdarg.h>
#include <stdio.h>
#include <syslog.h>
#include <cutils/properties.h>

#include "config_parser.h"
#include "ppd_log.h"

extern PowerPolicyDaemonConf *configuration; /* defined in ppd.h and declared in main.c */

static unsigned int log_opened; /* syslog already opened */

/*
 * Logger infrastructure. It reuses the same priorities as
 * sys/syslog.h because it's easier to manage.
 *
 *	LOG_EMERG	0
 *	LOG_ALERT	1
 *	LOG_CRIT	2
 *	LOG_ERR		3
 *	LOG_WARNING	4
 *	LOG_NOTICE	5
 *	LOG_INFO	6
 *	LOG_DEBUG	7
 *
 */

#define DLEVEL_PROP_NAME "ro.debug_level"
#define BTYPE_PROP_NAME "ro.build.type"
#define DLEVEL_PROP_LOW                 "0x4f4c"
#define DLEVEL_PROP_MID                 "0x494d"
#define DLEVEL_PROP_HIGH                "0x4948" 
// if( strncmp(debug_buf, DLEVEL_PROP_LOW, 6) == 0)  

int gv_check_prop = 0xFFFF;

int check_property(void)
{
	char debug_buf[32] = {0,};
	char build_buf[32] = {0,};
	
	if(gv_check_prop == 1)
		return 1;
	else if(gv_check_prop == 0)
		return 0;
	else
	{
		property_get(DLEVEL_PROP_NAME, debug_buf, NULL);
		property_get(BTYPE_PROP_NAME, build_buf, NULL);
		
		if ( ( strncmp(debug_buf, DLEVEL_PROP_LOW, 6) == 0) && ( strncmp(build_buf, "user", 4) == 0) )
		{
			gv_check_prop = 1;
			return 1;
		}
		else
		{
			gv_check_prop = 0;
			return 0;
		}
	}

}

void ppd_log(int prio, const char *fmt, ...) {
	va_list argp;
	va_list argp2;

	if( check_property() == 1 )
		return;

	/* do we need to write? */
	if (configuration->log_level < prio)
		return;

	va_start(argp, fmt);
	va_copy(argp2, argp);

	if (configuration->no_daemon) {
		if (configuration->log_level <= LOG_ERR) {
			vfprintf(stderr, fmt, argp);
			/* fflush(stderr); */
		} else {
			vfprintf(stdout, fmt, argp);
			/* fflush(stdout); */
		}
	} else {
		if (!log_opened) {
			/* open syslog */
			openlog("ppd", LOG_CONS, LOG_DAEMON);
			log_opened = 1;
		}
		vsyslog(prio, fmt, argp);
		if (configuration->log_level <= LOG_ERR) {
			vfprintf(stderr, fmt, argp2);
			/* fflush(stderr); */
		}
	}
	va_end(argp);
	va_end(argp2);
}
