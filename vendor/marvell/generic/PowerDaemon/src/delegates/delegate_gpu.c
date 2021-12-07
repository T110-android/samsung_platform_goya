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
#include <dirent.h>
#include "ppd_component.h"
#include "delegate_gpu.h"
#include "sysfs_gpu.h"
#include "libsysfs.h"

static struct gpu_global_data *delegate_gpu = &_delegate_gpu;
static int delegate_need_apply=0;

/* int get_gpu_num(void)
 *
 * Gets the number of installed GPUs from procfs
 * and sets gpu_num appropriately.
 *
 * Returns always at least 1 (you can't run this function without any gpu!)
 */
static unsigned int get_gpu_num(void) {
	char path[SYSFS_PATH_MAX];
	unsigned int unit = 0;
	DIR *dirptr = NULL;

	while(1) {
		snprintf(path, sizeof(path), "/sys/devices/platform/galcore/gpu/gpu%u/", unit);
		dirptr = opendir(path);
		if(dirptr) {
			closedir(dirptr);
			unit++;
		}
		else {
			break;
		}
	}

	//if no gpux sysfs at all, regard it at least have one
	if(unit == 0)
		unit = 1;

	return unit;
}

static int gpu_get_avail_freqs(unsigned int unit, char *avail_freqs) {
	char path[SYSFS_PATH_MAX];
	struct sysfs_attribute *attr;

	if(avail_freqs == NULL)
		return -1;

	snprintf(path, sizeof(path), "/sys/devices/platform/galcore/gpu/gpu%u/gpufreq/scaling_available_freqs", unit);
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

static int gpu_set_freq(unsigned int unit, unsigned long freq) {
    struct gpu_hardware_info *gpu_info = delegate_gpu->gpu_info;
    struct gpufreq_sys_info *sys_info;
	struct gpu_state state;

	state.id = unit;
	sys_info = gpu_info->sys_info+unit;
	state.minfreq = sys_info->min;
	state.maxfreq = sys_info->max;
	state.setspeed = freq;
	strcpy(state.governor, "userspace");
    if (sysfs_gpu_set_scaling_policy(&state) == 0) {
        clog(LOG_NOTICE, "set fix freq(%d) for gpu%d\n", freq, unit);
    }
    else {
        clog(LOG_WARNING, "Couldn't set fix freq(%d) for gpu%d\n", freq, unit);
        return -1;
    }

	return 0;
}

static int gpu_get_policy(unsigned int unit, unsigned long *minfreq,
	unsigned long *maxfreq, unsigned long *curfreq, char *governor) {

	struct gpu_state state;

	state.id = unit;
	if(sysfs_gpu_get_scaling_policy(&state) == 0) {
		clog(LOG_NOTICE, "gpu_get_policy for gpu%d\n", unit);
		*minfreq = state.minfreq;
		*maxfreq = state.maxfreq;
		*curfreq = state.setspeed;
		strcpy(governor, state.governor);
	}
	else {
		clog(LOG_WARNING, "Couldn't gpu_get_policy for gpu%d\n", unit);
		return -1;
	}

	return 0;
}

static unsigned long validate_frequency (struct gpufreq_sys_info *sys_info, unsigned long user_freq) {
    struct gpufreq_available_frequencies *tmp = sys_info->frequencies;
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

static int validate_governor (struct gpufreq_sys_info *sys_info, const char *governor) {
    struct gpufreq_available_governors *tmp = sys_info->governors;
    int found=0;

    /* find the governor */
    while (tmp != NULL) {
        if (strcmp(tmp->governor, governor) == 0) {
            found = 1;
            break;
        }
        tmp = tmp->next;
    }

    if(!found) {
        clog(LOG_ERR, "user governor [%s] not found in available governors\n", governor);
        return -1;
    }
    return 0;
}

static int set_gpu_frequency (struct gpu_state *state) {

    if((state == NULL) || (state->minfreq == 0) ||
        (state->maxfreq == 0) || state->governor[0] == 0) {
        clog(LOG_WARNING, "NULL gpu frequency parameters\n");
        return 0;
    }

    /* profile prechange event */

    if (sysfs_gpu_set_scaling_policy(state) == 0) {
        clog(LOG_NOTICE, "Constraint set for GPU%d (%d-%d-%s)\n", state->id,
			state->minfreq, state->maxfreq, state->governor);
    }
    else {
        clog(LOG_WARNING, "Couldn't set constraint for gpu%d (%d-%d-%s)\n",
                state->id, state->minfreq, state->maxfreq, state->governor);
        return -1;
    }

    /* profile postchange event */


    return 0;
}

/* gpu delegate APIs */
static int gpu_init(void) {
    unsigned int i=0;
    unsigned long min, max;
    struct gpu_hardware_info *gpu_info;

    delegate_gpu->gpu_info = malloc(sizeof(struct gpu_hardware_info));
    if (delegate_gpu->gpu_info == NULL) {
        return -1;
    }

    gpu_info = delegate_gpu->gpu_info;
    memset(gpu_info, 0, sizeof(struct gpu_hardware_info));

    /*
     *  read how many gpus are available here
     */
    gpu_info->gpus = get_gpu_num();

    /* store the result of merged ticket of each gpu */
    delegate_gpu->result = calloc(1, gpu_info->gpus * sizeof(struct gpu_state));
    if (delegate_gpu->result == NULL) {
        free(delegate_gpu->gpu_info);
        return -1;
    }

    /*
     *  find gpufreq information about each gpu
     */
    gpu_info->sys_info = calloc(1, gpu_info->gpus * sizeof(struct gpufreq_sys_info));
    if (gpu_info->sys_info == NULL) {
        clog(LOG_CRIT, "Unable to allocate memory (%s), exiting.\n", strerror(errno));
        free(delegate_gpu->gpu_info);
        free(delegate_gpu->result);
        return -1;
    }
    for (i = 0; i < gpu_info->gpus; i++) {
        if (sysfs_gpu_get_hardware_limits(i, &min, &max) != 0) {
            clog(LOG_WARNING, "Unable to get hardware frequency limits for GPU%d.\n", i);
        } else {
            (gpu_info->sys_info+i)->min = min;
            (gpu_info->sys_info+i)->max = max;
            clog(LOG_INFO, "Limits for gpu%d: MIN=%lu - MAX=%lu\n", i, min, max);
        }
        (gpu_info->sys_info+i)->governors = sysfs_gpu_get_available_governors(i);
        (gpu_info->sys_info+i)->frequencies = sysfs_gpu_get_available_frequencies(i);
    }

    return 0;
}

static int gpu_exit(void) {
    unsigned int i;
    struct gpu_hardware_info *gpu_info = delegate_gpu->gpu_info;
    struct ticket_obj *tmp_obj;

    /* free gpu_hardware_info */
    if (gpu_info != NULL) {
        clog(LOG_INFO, "freeing gpu_hardware_info.\n");
        if (gpu_info->sys_info != NULL) {
            for (i=0; i<gpu_info->gpus; i++) {
                if ((gpu_info->sys_info+i)->governors!=NULL)
                    sysfs_gpu_put_available_governors((gpu_info->sys_info+i)->governors);
                if ((gpu_info->sys_info+i)->frequencies!=NULL)
                    sysfs_gpu_put_available_frequencies((gpu_info->sys_info+i)->frequencies);
            }
            free(gpu_info->sys_info);
        }

        free(gpu_info);
        delegate_gpu->gpu_info = NULL;
    }

    /* free gpuTicketObj */
    if (LIST_EMPTY(&delegate_gpu->gpuTicketObj))
        return 0;
    clog(LOG_INFO, "freeing gpuTicketObj.\n");
    LIST_FOREACH_NODE(node, &delegate_gpu->gpuTicketObj) {
        tmp_obj = (struct ticket_obj*) node->content;
        if(tmp_obj->private_data)
            free(tmp_obj->private_data);
    }
    list_free_sublist(&(delegate_gpu->gpuTicketObj), delegate_gpu->gpuTicketObj.first);
    delegate_gpu->gpuTicketObj.first = delegate_gpu->gpuTicketObj.last = NULL;

    /* free result */
    if (delegate_gpu->result != NULL) {
        free(delegate_gpu->result);
    }

    return 0;
}

static int gpu_parse (void **obj, const char *key, const char *value) {
    struct gpu_hardware_info *gpu_info = delegate_gpu->gpu_info;
    struct gpufreq_sys_info *sys_info;
    struct NODE *n = NULL;
    struct ticket_obj *tmp_obj;
    struct gpu_state *tmp_state;
    unsigned long tmp_freq;
    unsigned int gpu_id;

    /* allocate memory for ticket obj */
    if(*obj == NULL) {
        n = node_new(NULL, sizeof(struct ticket_obj));
        if (n == NULL) {
            clog(LOG_ERR, "cannot make enough room for a new ticket OBJ (%s)\n",
                    strerror(errno));
            return -1;
        }
        tmp_obj = (struct ticket_obj *)n->content;
        tmp_obj->private_data = malloc(sizeof(struct gpu_state));;
        if(tmp_obj->private_data != NULL) {
            memset(tmp_obj->private_data, 0, sizeof(struct gpu_state));
            list_append(&delegate_gpu->gpuTicketObj, n);
            *obj = tmp_obj;
        }
        else {
            clog(LOG_ERR, "cannot make enough room for a new gpu_state struct (%s)\n",
                    strerror(errno));
            node_free(n);
            return -1;
        }
    }

    /* parse delegate setting */
    tmp_obj = (struct ticket_obj *)(*obj);
    clog(LOG_DEBUG, "parsing ticket obj 0x%x\n", tmp_obj);
    tmp_state = (struct gpu_state *)(tmp_obj->private_data);
    /* gpu_id must be the first element if has */
    if(strcmp(key,"gpu_id") == 0) {
        if(sscanf(value, "%ul", &gpu_id) != 1) {
            clog(LOG_ERR, "unable to parse <gpu_id> value %s.\n", value);
            return -1;
        }
        if(gpu_id > (gpu_info->gpus-1)) {
            clog(LOG_ERR, "invalid <gpu_id> value %s.\n", value);
            return -1;
        }
        tmp_state->id = gpu_id;
        clog(LOG_DEBUG, "GPU <gpu_id> [%s].\n", value);
        return 0;
    }

    sys_info = gpu_info->sys_info+tmp_state->id;
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
        clog(LOG_DEBUG, "GPU minfreq [%lu].\n", tmp_state->minfreq);
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
        clog(LOG_DEBUG, "GPU maxfreq [%lu].\n", tmp_state->maxfreq);
        return 0;
    }

    if(strcmp(key,"setspeed") == 0) {
        if(sscanf(value, "%lu", &tmp_freq) != 1) {
            clog(LOG_ERR, "unable to parse <setspeed> value %s.\n", value);
            return -1;
        }

		if (strstr(value, "%") != NULL) {
			clog(LOG_DEBUG, "transfer setspeed percent [%d%].\n", tmp_freq);
			tmp_freq = (sys_info->max) * ((float)tmp_freq / 100);
		}

        tmp_freq = validate_frequency(sys_info, tmp_freq);
        tmp_state->setspeed = tmp_freq;
        clog(LOG_DEBUG, "GPU setspeed [%lu].\n", tmp_state->setspeed);
        return 0;
    }

    if (strcmp(key,"governor") == 0) {
        if(0 == validate_governor(sys_info, value)){
            strncpy(tmp_state->governor, value, MAX_GOVERNOR_LEN - 1);
            clog(LOG_DEBUG, "GPU governor [%s].\n", tmp_state->governor);
        }
        return 0;
    }

    clog(LOG_DEBUG, "GPU not supported attributes.\n");
    return -1;

}

static int gpu_vote (int valid, unsigned int level, void *obj){
    struct ticket_obj *tmp_obj = (struct ticket_obj *)obj;
    if(tmp_obj->valid != valid) {
        tmp_obj->valid = valid;
        tmp_obj->level = level;
        delegate_need_apply++;
    }
    clog(LOG_DEBUG, "GPU voting obj=0x%x valid=%d level=%d.\n", tmp_obj, tmp_obj->valid, tmp_obj->level);
    return 0;
}

static int gpu_apply (int force) {
    struct ticket_obj *tmp_obj;
    struct gpu_state *tmp_state, *ret_state;
    unsigned int i;

    if (LIST_EMPTY(&delegate_gpu->gpuTicketObj)) {
        clog(LOG_DEBUG, "GPU ticket LIST is empty\n");
        return 0;
    }

    if ((!delegate_need_apply)&&(!force)) {
        clog(LOG_DEBUG, "GPU tickets no change, nothing to apply\n");
        return 0;
    }

    for (i=0; i<delegate_gpu->gpu_info->gpus; i++) {
        unsigned int level=0;
        ret_state = delegate_gpu->result+i;
        memset(ret_state, 0, sizeof(struct gpu_state));
        ret_state->id = i;

		/* go through all tickets:
		* replaced by higher level ticket,
		* merge with same level ticket,
		* ignore lower level ticket
		*/
        LIST_FOREACH_NODE(node, &delegate_gpu->gpuTicketObj) {
            tmp_obj = (struct ticket_obj*) node->content;
            tmp_state = (struct gpu_state *)tmp_obj->private_data;
            if(tmp_obj->valid && tmp_state->id == i) {
                if(tmp_obj->level > level) {
                    clog(LOG_DEBUG, "GPU Replace ticket private_data data\n");
                    level = tmp_obj->level;
                    ret_state->minfreq = tmp_state->minfreq;
                    ret_state->maxfreq = tmp_state->maxfreq;
                    ret_state->setspeed = tmp_state->setspeed;
                    memset(ret_state->governor, 0, MAX_GOVERNOR_LEN);
                    strcpy(ret_state->governor, tmp_state->governor);
                }

                if(tmp_obj->level == level) {
                    int p,q,r;
                    clog(LOG_DEBUG, "GPU Merge ticket private_data data\n");

                    /* the first time update NULL ret_state */
                    if((ret_state->minfreq == 0) || (ret_state->maxfreq == 0)
                        || ret_state->governor[0] == 0) {
                        clog(LOG_DEBUG, "the first time update NULL ret_state\n");
                        ret_state->minfreq = tmp_state->minfreq;
                        ret_state->maxfreq = tmp_state->maxfreq;
                        ret_state->setspeed = tmp_state->setspeed;
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

                    if(!ret_state->governor[0])
                        strcpy(ret_state->governor, tmp_state->governor);
                    else if(strcmp(ret_state->governor, tmp_state->governor)) {
                        for(r = 0; r < PRIO_GPUFREQ_MAX-1; r++)
                            if(!strcmp(ret_state->governor, supported_governors[r]))
                                break;
                        p = r;

                        for(r = 0; r < PRIO_GPUFREQ_MAX-1; r++)
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
        /* set gpu frequency */
        set_gpu_frequency(ret_state);
        delegate_need_apply = 0;
    }
    return 0;
}


static int gpu_free (void *obj) {

    return 0;
}

static PowerPolicyDaemonDelegateUtils gpu_utils = {
	.get_unit_num		= &get_gpu_num,
	.get_avail_freqs	= &gpu_get_avail_freqs,
	.set_fix_freq		= &gpu_set_freq,
	.get_current_policy	= &gpu_get_policy,
};

static PowerPolicyDaemonDelegate gpu = {
    .delegate_name		= "gpu",
    .delegate_init		= &gpu_init,
    .delegate_exit		= &gpu_exit,
    .delegate_parse		= &gpu_parse,
    .delegate_vote		= &gpu_vote,
    .delegate_apply		= &gpu_apply,
    .delegate_free		= &gpu_free,
	.data				= &gpu_utils,
};

PowerPolicyDaemonDelegate *create_delegate (void) {
    return &gpu;
}
