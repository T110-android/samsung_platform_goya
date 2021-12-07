/*
 *  Copyright (C) 2002-2008  Mattia Dongili <malattia@linux.it>
 *                           George Staikos <staikos@0wned.org>
 *  Copyright (C) 2012 Marvell, Inc.
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

#ifndef _CONFIG_PARSER_H
#define _CONFIG_PARSER_H

#include <stdio.h>
#include <pthread.h>
#include "ppd.h"
#include "ppd_component.h"
#include "list.h"
#include <libxml/tree.h>
#include <libxml/parser.h>

struct directive {
	void *obj;
	struct _PowerPolicyDaemon_Keyword *keyword;
	struct _PowerPolicyDaemon_Plugin *plugin;
};

struct ticket {
	void *obj;
	struct _PowerPolicyDaemon_Delegate *delegate;
};

typedef struct _PowerPolicyDaemon_Constraint {
	char name[MAX_STRING_LEN];
	struct _PowerPolicyDaemon_Rule *rule;
	int active;
	struct LIST tickets; /* list of struct ticket */
}PowerPolicyDaemonConstraint;

typedef struct _PowerPolicyDaemon_Rule {
	char rule_name[MAX_STRING_LEN];
	char constraint_name[MAX_STRING_LEN];
	PowerPolicyDaemonConstraint *constraint;
	struct LIST directives; /* list of struct directive */
	unsigned int directives_count;
	unsigned int level;
	int enable;
}PowerPolicyDaemonRule;

typedef struct _PowerPolicyDaemon_Conf {

	char config_file[MAX_PATH_LEN];
	char pidfile[MAX_PATH_LEN];
	int log_level;
	unsigned int enable_remote;
	gid_t remote_gid;
	unsigned int version;
	int operation_mode; /* operation mode (manual / dynamic) */
    struct timeval poll_intv;
    struct timeval timestamp;
	unsigned int has_sysfs;
	unsigned int no_daemon;
	unsigned int print_help;
	unsigned int print_version;
	unsigned int android_log;
	pthread_mutex_t ppd_mutex;
	xmlDocPtr doc;
	struct LIST rules; /* list of configured struct rule */
	struct LIST constraints; /* list of configured struct constraint */
	struct LIST plugins; /* list of configured plugins struct plugin */
	struct LIST delegates; /* list of configured struct delegate */
}PowerPolicyDaemonConf;

int	init_configuration	(PowerPolicyDaemonConf *config);
void	free_configuration	(PowerPolicyDaemonConf *config);
extern PowerPolicyDaemonConf * get_ppd_config(void);

#endif /* _CONFIG_PARSER_H */
