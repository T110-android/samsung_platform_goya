/*
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

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include "ppd_component.h"
#include "delegate_ddr.h"
#include "sysfs_ddr.h"
#include "libsysfs.h"

static struct ddr_global_data *delegate_ddr = &_delegate_ddr;
static int delegate_need_apply=0;
static char default_ddr_gov[32];

static unsigned int get_ddr_num(void) {
	return 1;
}

static int ddr_get_avail_freqs(unsigned int unit, char *avail_freqs) {
	char path[SYSFS_PATH_MAX];
	struct sysfs_attribute *attr;

	if(avail_freqs == NULL)
		return -1;

	snprintf(path, sizeof(path), "/sys/class/devfreq/devfreq-ddr/available_freqs");
	attr = sysfs_open_attribute(path);
	if (!attr) {
		return -1;
	}
	if (sysfs_read_attribute(attr)) {
		sysfs_close_attribute(attr);
		return -1;
	} else
		strcpy(avail_freqs, attr->value);
	sysfs_close_attribute(attr);

	return 0;
}

static int ddr_set_freq(unsigned int unit, unsigned long freq) {
	struct ddr_state state;

	state.poll_interval = 50;
	state.minfreq = state.maxfreq = freq;
	strcpy(state.governor, default_ddr_gov);
    if (sysfs_ddr_set_scaling_policy(&state) == 0) {
        clog(LOG_NOTICE, "ddr_set_freq for ddr\n");
    }
    else {
        clog(LOG_WARNING, "Couldn't set fix freq(%d) for ddr\n", freq);
        return -1;
    }

	return 0;
}

static int ddr_get_policy(unsigned int unit, unsigned long *minfreq,
	unsigned long *maxfreq, unsigned long *curfreq, char *governor) {

	struct ddr_state state;

	if(sysfs_ddr_get_scaling_policy(&state) == 0) {
		clog(LOG_NOTICE, "ddr_get_policy for ddr\n");
		*minfreq = state.minfreq;
		*maxfreq = state.maxfreq;
		*curfreq = sysfs_ddr_get_ddrinfo_curfreq();
		strcpy(governor, state.governor);
	}
	else {
		clog(LOG_WARNING, "Couldn't ddr_get_policy for ddr\n");
		return -1;
	}

	return 0;
}

static unsigned long validate_frequency (struct ddrfreq_sys_info *sys_info, unsigned long user_freq) {
    struct ddrfreq_available_frequencies *tmp = sys_info->frequencies;
    unsigned long ret_freq=0;
	unsigned long higher=0L, lower=0L;

    if (user_freq < sys_info->min) {
        clog(LOG_NOTICE, "minfreq below hardware MIN, resetting\n");
        ret_freq = sys_info->min;
        return ret_freq;
    }
    if (user_freq > sys_info->max) {
        clog(LOG_NOTICE, "minfreq above hardware MAX, resetting\n");
        ret_freq = sys_info->max;
        return ret_freq;
    }

    /* normalize the freq */
    while (tmp != NULL) {
	if (tmp->frequency>=user_freq && (tmp->frequency<higher || higher==0))
		higher = tmp->frequency;

	if (tmp->frequency<=user_freq && (tmp->frequency>lower || lower==0))
		lower = tmp->frequency;

        tmp = tmp->next;
    }

    ret_freq = (user_freq >= ((higher-lower)/2)+lower) ? higher : lower;
    return ret_freq;
}

static int validate_governor (struct ddrfreq_sys_info *sys_info, const char *governor) {
    struct ddrfreq_available_governors *tmp = sys_info->governors;
    int found=0;

    /* find the governor */
    while (tmp != NULL) {
        if (strcmp(tmp->governor, governor) == 0) {
            return 0;
        }
        tmp = tmp->next;
    }

    if(!found) {
        clog(LOG_ERR, "user governor [%s] not found in available governors\n", governor);
        return -1;
    }
    return 0;
}

static int set_ddr_frequency (struct ddr_state *state) {

    if((state == NULL) || (state->minfreq == 0) ||
        (state->maxfreq == 0) || state->governor[0] == 0) {
        clog(LOG_WARNING, "NULL ddr frequency parameters\n");
        return 0;
    }

    /* profile prechange event */

    if (sysfs_ddr_set_scaling_policy(state) == 0) {
        clog(LOG_NOTICE, "Constraint set for DDR (%d-%d-%d-%s)\n",
			state->minfreq, state->maxfreq, state->poll_interval, state->governor);
    }
    else {
        clog(LOG_WARNING, "Couldn't set constraint for ddr (%d-%d-%d-%s)\n",
                state->minfreq, state->maxfreq, state->poll_interval, state->governor);
        return -1;
    }

    /* profile postchange event */


    return 0;
}

/* ddr delegate APIs */
static int ddr_init(void) {
    unsigned long min, max;
    struct ddr_hardware_info *ddr_info;
    struct ddr_state state;

    delegate_ddr->ddr_info = malloc(sizeof(struct ddr_hardware_info));
    if (delegate_ddr->ddr_info == NULL) {
        return -1;
    }

    ddr_info = delegate_ddr->ddr_info;
    memset(ddr_info, 0, sizeof(struct ddr_hardware_info));

    /* store the result of merged ticket of each ddr */
    delegate_ddr->result = calloc(1, sizeof(struct ddr_state));
    if (delegate_ddr->result == NULL) {
        free(delegate_ddr->ddr_info);
        return -1;
    }

    /* find ddrfreq information about each ddr */
    ddr_info->sys_info = calloc(1, sizeof(struct ddrfreq_sys_info));
    if (ddr_info->sys_info == NULL) {
        clog(LOG_CRIT, "Unable to allocate memory (%s), exiting.\n", strerror(errno));
        free(delegate_ddr->ddr_info);
        free(delegate_ddr->result);
        return -1;
    }
    /* fix me while the ddr hardware freq limit sysfs nodes are added */
    if(sysfs_ddr_get_scaling_policy(&state) == 0) {
        min = state.minfreq;
        max = state.maxfreq;
        strcpy(default_ddr_gov, state.governor);
        clog(LOG_NOTICE, "ddr_init: DDR hardware frequency limitation(%d-%d)\n", min, max);
    }
    else {
        clog(LOG_WARNING, "ddr_init: Couldn't ddr_get_policy for ddr\n");
        return -1;
    }
    /* for z3, available freqs are 156, 312, 400MHz */
    (ddr_info->sys_info)->min = min;
    (ddr_info->sys_info)->max = max;
    (ddr_info->sys_info)->governors = sysfs_ddr_get_available_governors();
    (ddr_info->sys_info)->frequencies = sysfs_ddr_get_available_frequencies();

    return 0;
}

static int ddr_exit(void) {
    struct ddr_hardware_info *ddr_info = delegate_ddr->ddr_info;
    struct ticket_obj *tmp_obj;

    /* free ddr_hardware_info */
    if (ddr_info != NULL) {
        clog(LOG_INFO, "freeing ddr_hardware_info.\n");
        if (ddr_info->sys_info != NULL) {
            if ((ddr_info->sys_info)->governors!=NULL)
                sysfs_ddr_put_available_governors((ddr_info->sys_info)->governors);
            if ((ddr_info->sys_info)->frequencies!=NULL)
                sysfs_ddr_put_available_frequencies((ddr_info->sys_info)->frequencies);
            free(ddr_info->sys_info);
        }

        free(ddr_info);
        delegate_ddr->ddr_info = NULL;
    }

    /* free ddrTicketObj */
    if (LIST_EMPTY(&delegate_ddr->ddrTicketObj))
        return 0;
    clog(LOG_INFO, "freeing ddrTicketObj.\n");
    LIST_FOREACH_NODE(node, &delegate_ddr->ddrTicketObj) {
        tmp_obj = (struct ticket_obj*) node->content;
        if(tmp_obj->private_data)
            free(tmp_obj->private_data);
    }
    list_free_sublist(&(delegate_ddr->ddrTicketObj), delegate_ddr->ddrTicketObj.first);
    delegate_ddr->ddrTicketObj.first = delegate_ddr->ddrTicketObj.last = NULL;

    /* free result */
    if (delegate_ddr->result != NULL) {
        free(delegate_ddr->result);
    }

    return 0;
}

static int ddr_parse (void **obj, const char *key, const char *value) {
    struct ddr_hardware_info *ddr_info = delegate_ddr->ddr_info;
    struct ddrfreq_sys_info *sys_info;
    struct NODE *n = NULL;
    struct ticket_obj *tmp_obj;
    struct ddr_state *tmp_state;
    unsigned long tmp_freq;

    /* allocate memory for ticket obj */
    if(*obj == NULL) {
        n = node_new(NULL, sizeof(struct ticket_obj));
        if (n == NULL) {
            clog(LOG_ERR, "cannot make enough room for a new ticket OBJ (%s)\n",
                    strerror(errno));
            return -1;
        }
        tmp_obj = (struct ticket_obj *)n->content;
        tmp_obj->private_data = malloc(sizeof(struct ddr_state));;
        if(tmp_obj->private_data != NULL) {
            memset(tmp_obj->private_data, 0, sizeof(struct ddr_state));
            list_append(&delegate_ddr->ddrTicketObj, n);
            *obj = tmp_obj;
        }
        else {
            clog(LOG_ERR, "cannot make enough room for a new ddr_state struct (%s)\n",
                    strerror(errno));
            node_free(n);
            return -1;
        }
    }

    /* parse delegate setting */
    tmp_obj = (struct ticket_obj *)(*obj);
    clog(LOG_DEBUG, "parsing ticket obj 0x%x\n", tmp_obj);
    tmp_state = (struct ddr_state *)(tmp_obj->private_data);

    if(!tmp_state->governor[0]) {
        strcpy(tmp_state->governor, default_ddr_gov);
    }

    sys_info = ddr_info->sys_info;
    if(strcmp(key,"minfreq") == 0) {
        if(sscanf(value, "%lu", &tmp_freq) != 1) {
            clog(LOG_ERR, "unable to parse <minfreq> value %s.\n", value);
            return -1;
        }

		if (strstr(value, "%") != NULL) {
			clog(LOG_DEBUG, "transfer minfreq percent [%d%].\n", tmp_freq);
			tmp_freq = (sys_info->max) * ((float)tmp_freq / 100);
		}

        tmp_freq = validate_frequency(sys_info, tmp_freq);
        tmp_state->minfreq = tmp_freq;
        clog(LOG_DEBUG, "DDR minfreq [%lu].\n", tmp_state->minfreq);
        return 0;
    }

    if(strcmp(key,"maxfreq") == 0) {
        if(sscanf(value, "%lu", &tmp_freq) != 1) {
            clog(LOG_ERR, "unable to parse <maxfreq> value %s.\n", value);
            return -1;
        }

		if (strstr(value, "%") != NULL) {
			clog(LOG_DEBUG, "transfer maxfreq percent [%d%].\n", tmp_freq);
			tmp_freq = (sys_info->max) * ((float)tmp_freq / 100);
		}

        tmp_freq = validate_frequency(sys_info, tmp_freq);
        tmp_state->maxfreq = tmp_freq;
        clog(LOG_DEBUG, "DDR maxfreq [%lu].\n", tmp_state->maxfreq);
        return 0;
    }

    if (strcmp(key,"interval") == 0) {
        if(sscanf(value, "%lu", &tmp_state->poll_interval) != 1) {
            clog(LOG_ERR, "unable to parse <interval> value %s.\n", value);
            return -1;
        }
        clog(LOG_DEBUG, "DDR poll interval [%lu].\n", tmp_state->poll_interval);
        return 0;
    }

    if (strcmp(key,"governor") == 0) {
        if(0 == validate_governor(sys_info, value)){
            strncpy(tmp_state->governor, value, MAX_GOVERNOR_LEN - 1);
            clog(LOG_DEBUG, "DDR governor [%s].\n", tmp_state->governor);
        }
        return 0;
    }

    clog(LOG_DEBUG, "DDR not supported attributes.\n");
    return -1;

}

static int ddr_vote (int valid, unsigned int level, void *obj){
    struct ticket_obj *tmp_obj = (struct ticket_obj *)obj;
    if(tmp_obj->valid != valid) {
        tmp_obj->valid = valid;
        tmp_obj->level = level;
        delegate_need_apply++;
    }
    clog(LOG_DEBUG, "DDR voting obj=0x%x valid=%d level=%d.\n", tmp_obj, tmp_obj->valid, tmp_obj->level);
    return 0;
}

static int ddr_apply (int force) {
    struct ticket_obj *tmp_obj;
    struct ddr_state *tmp_state, *ret_state;

    if (LIST_EMPTY(&delegate_ddr->ddrTicketObj)) {
        clog(LOG_DEBUG, "DDR ticket LIST is empty\n");
        return 0;
    }

    if ((!delegate_need_apply)&&(!force)) {
        clog(LOG_DEBUG, "DDR tickets no change, nothing to apply\n");
        return 0;
    }

    {
        unsigned int level=0;
        ret_state = delegate_ddr->result;
        memset(ret_state, 0, sizeof(struct ddr_state));

		/* go through all tickets:
		* replaced by higher level ticket,
		* merge with same level ticket,
		* ignore lower level ticket
		*/
        LIST_FOREACH_NODE(node, &delegate_ddr->ddrTicketObj) {
            tmp_obj = (struct ticket_obj*) node->content;
            tmp_state = (struct ddr_state *)tmp_obj->private_data;
            if(tmp_obj->valid) {
                if(tmp_obj->level > level) {
                    clog(LOG_DEBUG, "DDR Replace ticket private_data data\n");
                    level = tmp_obj->level;
                    ret_state->minfreq = tmp_state->minfreq;
                    ret_state->maxfreq = tmp_state->maxfreq;
                    ret_state->poll_interval = tmp_state->poll_interval;
                    memset(ret_state->governor, 0, MAX_GOVERNOR_LEN);
                    strcpy(ret_state->governor, tmp_state->governor);
                }

                if(tmp_obj->level == level) {
                    int p,q,r;
                    clog(LOG_DEBUG, "DDR Merge ticket private_data data\n");

                    /* the first time update NULL ret_state */
                    if((ret_state->minfreq == 0) || (ret_state->maxfreq == 0)
                        || ret_state->governor[0] == 0) {
                        clog(LOG_DEBUG, "the first time update NULL ret_state\n");
                        ret_state->minfreq = tmp_state->minfreq;
                        ret_state->maxfreq = tmp_state->maxfreq;
                        ret_state->poll_interval = tmp_state->poll_interval;
                        memset(ret_state->governor, 0, MAX_GOVERNOR_LEN);
                        strcpy(ret_state->governor, tmp_state->governor);
                        continue;
                    }

                    /*
                     * Currently, for Power Saving issue: We would make max limit
                     * requirement as higher priority, but boosting as max as we could
                     */

                    /* Confilct case: Performance requirement */
                    if((tmp_state->minfreq > ret_state->maxfreq)){
                        continue;
                    }

                    /* Confilct case: Powersaving requirement */
                    if(tmp_state->maxfreq < ret_state->minfreq) {
                        ret_state->maxfreq = tmp_state->maxfreq;
                        ret_state->minfreq = tmp_state->maxfreq;
                        continue;
                    }

                    if(tmp_state->minfreq > ret_state->minfreq)
                        ret_state->minfreq = tmp_state->minfreq;

                    if(tmp_state->maxfreq < ret_state->maxfreq)
                        ret_state->maxfreq = tmp_state->maxfreq;

                    if(tmp_state->poll_interval > ret_state->poll_interval)
                        ret_state->poll_interval = tmp_state->poll_interval;

                    if(!ret_state->governor[0])
                        strcpy(ret_state->governor, tmp_state->governor);
                    else if(strcmp(ret_state->governor, tmp_state->governor)) {
                        for(r = 0; r < PRIO_DDRFREQ_MAX-1; r++)
                            if(!strcmp(ret_state->governor, supported_governors[r]))
                                break;
                        p = r;

                        for(r = 0; r < PRIO_DDRFREQ_MAX-1; r++)
                            if(!strcmp(tmp_state->governor, supported_governors[r]))
                                break;
                        q = r;

                        /* Replace with the governor with a higher priority */
                        if(q > p) {
                            memset(ret_state->governor, 0, MAX_GOVERNOR_LEN);
                            strcpy(ret_state->governor, tmp_state->governor);
                        }
                    }
                }
            }
        }
        /* set ddr frequency */
        set_ddr_frequency(ret_state);
        delegate_need_apply = 0;
    }
    return 0;
}


static int ddr_free (void *obj) {

    return 0;
}

static PowerPolicyDaemonDelegateUtils ddr_utils = {
	.get_unit_num		= &get_ddr_num,
	.get_avail_freqs	= &ddr_get_avail_freqs,
	.set_fix_freq		= &ddr_set_freq,
	.get_current_policy	= &ddr_get_policy,
};

static PowerPolicyDaemonDelegate ddr = {
    .delegate_name      = "ddr",
    .delegate_init      = &ddr_init,
    .delegate_exit      = &ddr_exit,
    .delegate_parse     = &ddr_parse,
    .delegate_vote      = &ddr_vote,
    .delegate_apply     = &ddr_apply,
    .delegate_free      = &ddr_free,
    .data				= &ddr_utils,
};

PowerPolicyDaemonDelegate *create_delegate (void) {
    return &ddr;
}
