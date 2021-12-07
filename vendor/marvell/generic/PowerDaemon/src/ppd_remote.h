/*
 *  Copyright (C) 2005  Mattia Dongili <malattia@linux.it>
 *                      Hrvoje Zeba <hrvoje@boo.mi2.hr>
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

#ifndef __PPD_REMOTE_H
#define __PPD_REMOTE_H

/*
 * Format:
 *	it is a structure to store the cmd
 *
 *	type:	command type
 *	argc:	the arg number for specific type
 *	args:	the detail of the args, in char format
 *
 *	e.g.:	for CMD_SET_FREQ (<delegates type><delegates Id><freq>)
 *			you should construct the structure like below:
 *
 *			type: CMD_SET_FREQ;
 *			argc: 3
 *			args: {cpu 0 156000}
 */
#define MAX_ARGS_LENGTH 128
struct _command{
	int type;
	int argc;
	char args[MAX_ARGS_LENGTH];
};
typedef struct _command Command;

#define CMD_UPDATE_STATE            1 /* no arguments */

#define CMD_SET_CONSTRAINT          2 /* <profile index> */
#define CMD_LIST_CONSTRAINTS        3 /* no arguments */
#define CMD_CUR_CONSTRAINTS         4 /* no argument */

#define CMD_SET_RULE                5 /* <rule name> */
#define CMD_CLEAR_RULE                6 /* <rule name> */
#define CMD_LIST_RULES              7 /* no arguments */
#define CMD_CUR_RULE                8 /* no arguments */

#define CMD_SET_PPD_MODE            9 /* <mode> */
#define CMD_GET_PPD_MODE            10 /* no arguments */

#define CMD_CUR_PROFILE             11 /* <delegates type><delegates Id> */
#define CMD_SET_FREQ                12 /* <delegates type><delegates Id><freq> */
#define CMD_GET_SCALING_AVAIL_FREQS 13 /* <delegates type><delegates Id> */

#define CMD_SET_LOG_LEVEL           14 /* <level> */


/* PPD RUN MODE */
#define MODE_MANUAL     (0)
#define MODE_DYNAMIC    (1)

#define INVALID_CMD     (int)0xffffffff

#endif

