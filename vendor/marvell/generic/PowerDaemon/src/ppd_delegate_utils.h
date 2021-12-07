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

#ifndef __DELEGATE_UTILS_H__
#define __DELEGATE_UTILS_H__

#include "ppd_component.h"
#include "list.h"

struct delegate_obj {
	char name[256];
	void *library;
	PowerPolicyDaemonDelegate *delegate;
	unsigned int used;
	unsigned int configured;
};

void discover_delegates(struct LIST *delegates);
void load_delegate_list(struct LIST *delegates);
int validate_delegates(struct LIST *delegates);
int load_delegate(struct delegate_obj *obj);
void close_delegate(struct delegate_obj *obj);
int get_delegate_object(struct delegate_obj *obj);
int initialize_delegate(struct delegate_obj *obj);
int finalize_delegate(struct delegate_obj *obj);

#endif
