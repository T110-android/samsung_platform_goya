/*
 *  Copyright (C) 2002-2005  Mattia Dongili <malattia@linux.it>
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

#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fnmatch.h>
#include <stdio.h>
#include <string.h>
#include "ppd.h"
#include "ppd_log.h"
#include "ppd_delegate_utils.h"

static int ppd_delegate_filter(const struct dirent *d) {
	return fnmatch("delegate_*.so", d->d_name, 0) == 0;
}

/*
 * Try to discover pugins
 */
void discover_delegates(struct LIST *delegates) {
	int n = 0;
	struct delegate_obj o_delegate;
	struct NODE *n_delegate;
	struct dirent **namelist;

	/* delegate names */
	n = scandir(PPD_LIBDIR, &namelist, ppd_delegate_filter, NULL);
	if (n > 0) {
		while (n--) {
			o_delegate.library = NULL;
			o_delegate.delegate = NULL;
			o_delegate.used = 0;
			o_delegate.configured = 0;

			sscanf(namelist[n]->d_name, "delegate_%[^.].so", o_delegate.name);
			o_delegate.name[MAX_STRING_LEN-1] = '\0';

			n_delegate = node_new(&o_delegate, sizeof(struct delegate_obj));
			list_append(delegates, n_delegate);
			clog(LOG_INFO, "found delegate: %s\n", o_delegate.name);

			free(namelist[n]);
		}
		free(namelist);

	} else if (n < 0) {
		clog(LOG_ERR, "error reading %s: %s\n",
				PPD_LIBDIR, strerror(errno));

	} else {
		clog(LOG_WARNING, "no delegate found in %s\n", PPD_LIBDIR);
	}
}

/*
 *  Load delegates from a list of delegate_obj's. Also cleanup the
 *  list if a delegate fails to load
 */

void load_delegate_list(struct LIST *delegates) {
	struct delegate_obj *o_delegate = NULL;
	struct NODE *n = NULL;

	n = delegates->first;
	while (n != NULL) {
		o_delegate = (struct delegate_obj*)n->content;
		/* take care!! if statement badly indented!! */
		if (load_delegate(o_delegate) == 0 &&
				get_delegate_object(o_delegate) == 0 &&
				initialize_delegate(o_delegate) == 0) {
			clog(LOG_INFO, "delegate loaded: %s\n", o_delegate->delegate->delegate_name);
			n = n->next;

		} else {
			clog(LOG_INFO, "delegate failed to load: %s\n", o_delegate->name);
			/* remove the list item and assing n the next node (returned from list_remove_node) */
			clog(LOG_NOTICE, "discarded delegate %s\n", o_delegate->name);
			n = list_remove_node(delegates, n);
		} /* end else */
	} /* end while */
}

/* Validate delegates after parsing the configuration, an unused
 * delegate is unloaded and removed from the list.
 *
 * Returns the number of remaining delegates.
 */
int validate_delegates(struct LIST *delegates) {
	struct delegate_obj *o_delegate = NULL;
	struct NODE *n = NULL;
	int used_delegates = 0;

	n = delegates->first;
	while (n != NULL) {
		o_delegate = (struct delegate_obj*)n->content;
		if (o_delegate->used != 0) {
			used_delegates++;
			n = n->next;
		} else {
			clog(LOG_INFO, "%s delegate is unused.\n", o_delegate->name);
			finalize_delegate((struct delegate_obj*)n->content);
			close_delegate((struct delegate_obj*)n->content);
			n = list_remove_node(delegates, n);
		}
	}
	return used_delegates;
}

/*  int load_delegate(struct delegate_obj *obj)
 *  Open shared libraries
 */
int load_delegate(struct delegate_obj *obj) {
	char libname[512];

	snprintf(libname, 512, PPD_LIBDIR"delegate_%s.so", obj->name);

	clog(LOG_INFO, "Loading \"%s\" for delegate \"%s\".\n", libname, obj->name);
	obj->library = dlopen(libname, RTLD_LAZY);
	if (!obj->library) {
		clog(LOG_ERR, "%s\n", dlerror());
		return -1;
	}

	return 0;
}

/*  void close_delegate(struct delegate_obj *obj)
 *  Close shared libraries
 */
void close_delegate(struct delegate_obj *obj) {
	/* close library */
	if (dlclose(obj->library) != 0) {
		clog(LOG_ERR, "Error unloading delegate %s: %s\n", obj->name, dlerror());
		return;
	}
	clog(LOG_INFO, "%s delegate closed.\n", obj->name);
}

/*  int get_delegate_object(struct delegate_obj *obj)
 *  Calls the create_delegate routine.
 */
int get_delegate_object(struct delegate_obj *obj) {

	/* pointer to an error message, if any */
	const char* error;
	/* delegate ptr */
	PowerPolicyDaemonDelegate *(*create)(void);

	clog(LOG_INFO, "Getting delegate object for \"%s\".\n", obj->name);
	/* create delegate */
	create = (PowerPolicyDaemonDelegate * (*) (void))dlsym(obj->library, "create_delegate");
	error = dlerror();
	if (error) {
		clog(LOG_ERR, "get_delegate_object(): %s\n", error);
		return -1;
	}
	obj->delegate = create();

	return 0;
}

/*  int initialize_delegate(struct delegate_obj *obj)
 *  Call delegate_init()
 */
int initialize_delegate(struct delegate_obj *obj) {
	int ret = 0;
	clog(LOG_INFO, "Initializing delegate \"%s-%s\".\n",
			obj->name, obj->delegate->delegate_name);
	/* call init function */
	if (obj->delegate->delegate_init != NULL) {
		ret = obj->delegate->delegate_init();
	}
	return ret;
}

/*  int finalize_delegate(struct delegate_obj *obj)
 *  Call delegate_exit()
 */
int finalize_delegate(struct delegate_obj *obj) {
	if (obj != NULL && obj->delegate->delegate_exit != NULL) {
		clog(LOG_INFO, "Finalizing delegate \"%s-%s\".\n",
				obj->name, obj->delegate->delegate_name);
		/* call exit function */
		obj->delegate->delegate_exit();
		return -1;
	}
	return 0;
}

