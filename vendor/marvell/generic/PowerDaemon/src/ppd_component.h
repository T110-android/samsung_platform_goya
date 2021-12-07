/*
 *  Copyright (C) 2002-2006  Mattia Dongili <malattia@linux.it>
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

#ifndef __PPD_COMPONENT_H__
#define __PPD_COMPONENT_H__

#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include "ppd.h"
#include "ppd_remote.h"
#include "config_parser.h"
#include "ppd_log.h"

#define FALSE	0
#define TRUE	1
#define DONT_MATCH  0
#define MATCH       1

#define wakeup_ppd()    kill(getpid(), SIGALRM)
#define kill_ppd()      kill(getpid(), SIGTERM)

/*
 *  A ppd keyword consists of the proper word to match at the
 *  beginning of the Rule line. The struct consists of two other function
 *  pointers, one will provide the function to be called if the keyword
 *  being considered matches with *word, the sencond will provide a function to
 *  be called during the main loop that will evaluate the current
 *  system state (as read by the same plugin) against it.
 *
 *  At least one out of evaluate, profile_pre_change, profile_post_change,
 *  rule_pre_change, rule_post_change MUST be defined for a single keyword.
 *  Otherwise it doesn't make sense...
 */
typedef struct _PowerPolicyDaemon_Keyword {

	/* The word that is managed.
	 *
	 * Can't be NULL.
	 */
	const char *word;

	/* Whether the word is updated.
	 *
	 */
	int isupdate;

	/* Whether the word is always in poll loop.
	 *
	 */
	int polling;

	/* function pointer to the keyword parser. line is a config file _value_
	 * (as in key=value) and obj must be assigned a structure that will be
	 * used by the evaulate functioned
	 *
	 * Must be non-NULL.
	 */
	int (*parse) (const char *line, void **obj);

	/* function pointer to the evaluator. obj is the structure provided by
	 * the parse function and that represent the system state that must eventually
	 * be matched. If the system state matches the function must return MATCH (1)
	 * otherwise DONT_MATCH (0).
	 *
	 * Can be NULL.
	 */
	int (*evaluate) (const void *obj);

	/* function pointer to the profile_pre_change event. obj is the structure
	 * previously provided by the parse function, old and new are the old
	 * and new policy pointer respctively.
	 * The function is called prior to the call to set_policy() when a new
	 * Profile is going to be set.
	 *
	 * Can be NULL.
	 */
	void (*profile_pre_change) (void *obj);

	/* function pointer to the profile_post_change event. The same as
	 * profile_pre_change applies except for the fact that everything is
	 * referred tto _after_ set_policy() has been called.
	 *
	 * Can be NULL.
	 */
	void (*profile_post_change) (void *obj);

	/* function pointer to the rule_pre_change event. obj is the structure
	 * previously provided by the parse function, old and new are the old
	 * and new policy pointer respctively.
	 * The function is called prior to the call to set_policy() when a new Rule
	 * applies the current system state. Note however that set_policy() will not
	 * be called if the Profile doesn't change (you can tell that by comparing the
	 * old and new policy pointers, if they are the same then set_policy() won't
	 * be called).
	 *
	 * Can be NULL.
	 */
	void (*rule_pre_change) (void *obj);

	/* function pointer to the rule_post_change event. The same as
	 * rule_pre_change applies except for the fact that everything is
	 * referred tto _after_ set_policy() has been called.
	 *
	 * Can be NULL.
	 */
	void (*rule_post_change) (void *obj);


	/* Allows the owner to define a specific function to be called when freeing
	 * malloced during the 'parse' call. Not required, if missing a libc call to
	 * 'free' is performed with the same obj argument.
	 *
	 * Can be NULL.
	 */
	void (*free) (void *obj);
}PowerPolicyDaemonKeyword;

/*
 *  A PowerPolicyDaemon plugin is a collection of functions and settings able to
 *  monitor some kind of system resource/state and tell if the present
 *  state is conformant to the one configured.
 *  PowerPolicyDaemon plugins must be decalared static to avoid symbol clashes.
 */
typedef struct _PowerPolicyDaemon_Plugin {

	/****************************************
	 *  PLUGIN IDENTIFICATION AND SETTINGS  *
	 ****************************************/
	/* plugin name, must be unique */
	const char *plugin_name;

	/* plugin tag, must be unique */
	unsigned int plugin_tag;

	/* array of keywords handled by this plugin */
	PowerPolicyDaemonKeyword *keywords;

	/************************
	 *  FUNCTION POINTERS   *
	 ************************/
	/* Plugin intialization */
	int (*plugin_init) (void);

	/* Plugin cleanup */
	int (*plugin_exit) (void);

	/* Update plugin data */
	int (*plugin_update) (void *data);

	/* Plugin configuration */
	int (*plugin_conf) (const char *key, const char *value);

	/* Plugin post configuration
	 * This will be called after the configuration of the plugin
	 * is performed, that is IFF a section with the plugin name
	 * as been found and parsed succesfully.
	 */
	int (*plugin_post_conf) (void);

	/* Plugin socket connection release callback
	 * This will be called when the socket connection with
	 * the plugin is closed.
	 *
	 * Can be NULL.
	 */
	int (*plugin_sock_release) (int tid);

	/* Allow plugins to make some data available to others.
	 * This data can be retrieved using
	 * void *get_plugin_data(const char *name)
	 * exported by the core ppd.
	 */
	void *data;
}PowerPolicyDaemonPlugin;

/*
 *  A PowerPolicyDaemon_plugin MUST define the following function to provide the
 *  core PowerPolicyDaemon with the correct struct PowerPolicyDaemon_plugin structure
 */
PowerPolicyDaemonPlugin *create_plugin(void);

struct ticket_obj {
	int valid;
	unsigned int level;
	void *private_data;
};

/*
 *  A ppd plugin is a collection of functions and settings able to
 *  monitor some kind of system resource/state and tell if the present
 *  state is conformant to the one configured.
 *  ppd plugins must be decalared static to avoid symbol clashes.
 */
typedef struct _PowerPolicyDaemon_Delegate {

	/* delegate name, must be unique */
	const char *delegate_name;

	/************************
	 *  FUNCTION POINTERS   *
	 ************************/
	/* delegate intialization */
	int (*delegate_init) (void);

	/* delegate cleanup */
	int (*delegate_exit) (void);

	/* delegate parse */
	int (*delegate_parse) (void **obj, const char *key, const char *value);

	/* delegate vote the ticket */
	int (*delegate_vote) (int valid, unsigned int level, void *obj);

	/* delegate ticket merge and apply */
	int (*delegate_apply) (int force);

	/* delegate free the allocated resources */
	int (*delegate_free) (void *obj);

	/* export some data in case */
	void *data;
}PowerPolicyDaemonDelegate;

/*
 *  A PowerPolicyDaemon_delegate MUST define the following function to provide the
 *  core PowerPolicyDaemon with the correct struct PowerPolicyDaemon_delegate structure
 */
PowerPolicyDaemonDelegate *create_delegate(void);

/* for command socket usage */
typedef struct _PowerPolicyDaemon_Delegate_Utils {

	/* get delegate unit number */
	unsigned int (*get_unit_num) (void);

	/* get avail freqs of a unit */
	int (*get_avail_freqs) (unsigned int unit, char *avail_freqs);

	/* set the fixed freq of a unit */
	int (*set_fix_freq) (unsigned int unit, unsigned long freq);

	/* get the current policy of the unit */
	int (*get_current_policy) (unsigned int unit, unsigned long *minfreq,
        unsigned long *maxfreq, unsigned long *curfreq, char *governor);

}PowerPolicyDaemonDelegateUtils;

#endif
