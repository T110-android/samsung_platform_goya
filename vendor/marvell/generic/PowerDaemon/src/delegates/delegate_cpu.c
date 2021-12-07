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
#include "delegate_cpu.h"
#include "sysfs_cpu.h"
#include "libsysfs.h"

#define POSSIBLE_CPU_NUM "/sys/devices/system/cpu/possible"

static struct cpu_global_data *delegate_cpu = &_delegate_cpu;
static int delegate_need_apply=0;

/* int get_cpu_num(void)
 *
 * Gets the number of installed CPUs from procfs
 * and sets cpu_num appropriately.
 *
 * Returns always at least 1 (you can't run this function without any cpu!)
 */
static unsigned int get_cpu_num(void) {
    FILE *fp;
    unsigned int n;
    char line[256];

    fp = fopen(POSSIBLE_CPU_NUM, "r");
    if(!fp) {
        clog(LOG_ERR, "%s: %s\n", POSSIBLE_CPU_NUM, strerror(errno));
        return 1;
    }

    n = 0;
    while(!feof(fp)) {
        fgets(line, 255, fp);
        sscanf(line, "0-%d", &n);
    }
    fclose(fp);

    clog(LOG_DEBUG, "found %i CPUs\n", n + 1);

    return n + 1;
}

static int cpu_get_avail_freqs(unsigned int unit, char *avail_freqs) {
    char path[SYSFS_PATH_MAX];
    struct sysfs_attribute *attr;

    if(avail_freqs == NULL)
        return -1;

    snprintf(path, sizeof(path), "/sys/devices/system/cpu/cpu%u/cpufreq/scaling_available_frequencies", unit);
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

static int cpu_set_freq(unsigned int unit, unsigned long freq) {
    struct cpu_hardware_info *cpu_info = delegate_cpu->cpu_info;
    struct cpufreq_sys_info *sys_info;
    struct cpu_state state;

    state.id = unit;
    sys_info = cpu_info->sys_info+unit;
    state.minfreq = sys_info->min;
    state.maxfreq = sys_info->max;
    state.setspeed = freq;
    strcpy(state.governor, "userspace");
    if (sysfs_cpu_set_scaling_policy(&state) == 0) {
        clog(LOG_NOTICE, "set fix freq(%d) for cpu%d\n", freq, unit);
    }
    else {
        clog(LOG_WARNING, "Couldn't set fix freq(%d) for cpu%d\n", freq, unit);
        return -1;
    }

    return 0;
}

static int cpu_get_policy(unsigned int unit, unsigned long *minfreq,
    unsigned long *maxfreq, unsigned long *curfreq, char *governor) {

    struct cpu_state state;

    state.id = unit;
    if(sysfs_cpu_get_scaling_policy(&state) == 0) {
        clog(LOG_NOTICE, "cpu_get_policy for cpu%d\n", unit);
        *minfreq = state.minfreq;
        *maxfreq = state.maxfreq;
        *curfreq = state.setspeed;
        strcpy(governor, state.governor);
    }
    else {
        clog(LOG_WARNING, "Couldn't cpu_get_policy for cpu%d\n", unit);
        return -1;
    }

    return 0;
}

static unsigned long validate_frequency (struct cpufreq_sys_info *sys_info, unsigned long user_freq) {
    struct cpufreq_available_frequencies *tmp = sys_info->frequencies;
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

static int validate_governor (struct cpufreq_sys_info *sys_info, const char *governor) {
    struct cpufreq_available_governors *tmp = sys_info->governors;
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

static int set_cpu_online(struct cpu_state *state){
    clog(LOG_NOTICE, "Constraint set for CPU%d online(%d)\n", state->id,
        state->online);
    //cpu0 always online, ignore it
    if(state->id == 0)
        return 0;
    else
        return sysfs_cpu_set_online_status(state);
}

static int get_cpu_online(struct cpu_state *state){
    int online = 0;
    sysfs_cpu_get_online_status(state, &online);
    return online;
}

static int set_cpu_frequency (struct cpu_state *state) {

    if((state == NULL) || (state->minfreq == 0) || 
        (state->maxfreq == 0) || state->governor[0] == 0) {
        clog(LOG_WARNING, "NULL cpu frequency parameters\n");
        return 0;
    }

    /* profile prechange event */

    if (sysfs_cpu_set_scaling_policy(state) == 0) {
        clog(LOG_NOTICE, "Constraint set for CPU%d (%d-%d-%s)\n", state->id,
            state->minfreq, state->maxfreq, state->governor);
        clog(LOG_NOTICE, "Constraint set for CPU%d governor threshold(%d-%d)\n", state->id,
            state->gov_para.down_threshold, state->gov_para.up_threshold);
        clog(LOG_NOTICE, "Constraint set for CPU%d governor sampling_rate(%d)\n", state->id,
            state->gov_para.sampling_rate);
    }
    else {
        clog(LOG_WARNING, "Couldn't set constraint for cpu%d (%d-%d-%s)\n",
                state->id, state->minfreq, state->maxfreq, state->governor);
        return -1;
    }

    /* profile postchange event */


    return 0;
}

static int set_cpu_hotplug_lock (int state) {
    clog(LOG_NOTICE, "Constraint set for CPU lock(%d)\n", state);
    return sysfs_cpu_set_hotplug_status(state);
}

static int get_cpu_hotplug_lock (void) {
    int lock_status = -1;
    sysfs_cpu_get_hotplug_status(&lock_status);
    return lock_status;
}

/* cpu delegate APIs */
static int cpu_init(void) {
    unsigned int i=0;
    unsigned long min, max;
    struct cpu_hardware_info *cpu_info;

    delegate_cpu->cpu_info = malloc(sizeof(struct cpu_hardware_info));
    if (delegate_cpu->cpu_info == NULL) {
        return -1;
    }

    cpu_info = delegate_cpu->cpu_info;
    memset(cpu_info, 0, sizeof(struct cpu_hardware_info));

    /*
     *  read how many cpus are available here
     */
    cpu_info->cpus = get_cpu_num();

    /* store the result of merged ticket of each cpu */
    delegate_cpu->result = malloc(cpu_info->cpus * sizeof(struct cpu_state));
    if (delegate_cpu->result == NULL) {
        free(delegate_cpu->cpu_info);
        return -1;
    }

    /*
     *  find cpufreq information about each cpu
     */
    cpu_info->sys_info = malloc(cpu_info->cpus * sizeof(struct cpufreq_sys_info));
    if (cpu_info->sys_info == NULL) {
        clog(LOG_CRIT, "Unable to allocate memory (%s), exiting.\n", strerror(errno));
        free(delegate_cpu->cpu_info);
        free(delegate_cpu->result);
        return -1;
    }
    for (i = 0; i < cpu_info->cpus; i++) {
        if (sysfs_cpu_get_hardware_limits(i, &min, &max) != 0) {
            clog(LOG_WARNING, "Unable to get hardware frequency limits for CPU%d.\n", i);
        } else {
            (cpu_info->sys_info+i)->min = min;
            (cpu_info->sys_info+i)->max = max;
            clog(LOG_INFO, "Limits for cpu%d: MIN=%lu - MAX=%lu\n", i, min, max);
        }
        (cpu_info->sys_info+i)->governors = sysfs_cpu_get_available_governors(i);
        (cpu_info->sys_info+i)->frequencies = sysfs_cpu_get_available_frequencies(i);
    }

    return 0;
}

static int cpu_exit(void) {
    unsigned int i;
    struct cpu_hardware_info *cpu_info = delegate_cpu->cpu_info;
    struct ticket_obj *tmp_obj;

    /* free cpu_hardware_info */
    if (cpu_info != NULL) {
        clog(LOG_INFO, "freeing cpu_hardware_info.\n");
        if (cpu_info->sys_info != NULL) {
            for (i=0; i<cpu_info->cpus; i++) {
                if ((cpu_info->sys_info+i)->governors!=NULL)
                    sysfs_cpu_put_available_governors((cpu_info->sys_info+i)->governors);
                if ((cpu_info->sys_info+i)->frequencies!=NULL)
                    sysfs_cpu_put_available_frequencies((cpu_info->sys_info+i)->frequencies);
            }
            free(cpu_info->sys_info);
        }

        free(cpu_info);
        delegate_cpu->cpu_info = NULL;
    }

    /* free cpuTicketObj */
    if (LIST_EMPTY(&delegate_cpu->cpuTicketObj))
        return 0;
    clog(LOG_INFO, "freeing cpuTicketObj.\n");
    LIST_FOREACH_NODE(node, &delegate_cpu->cpuTicketObj) {
        tmp_obj = (struct ticket_obj*) node->content;
        if(tmp_obj->private_data)
            free(tmp_obj->private_data);
    }
    list_free_sublist(&(delegate_cpu->cpuTicketObj), delegate_cpu->cpuTicketObj.first);
    delegate_cpu->cpuTicketObj.first = delegate_cpu->cpuTicketObj.last = NULL;

    /* free result */
    if (delegate_cpu->result != NULL) {
        free(delegate_cpu->result);
    }    

    return 0;
}

static int cpu_parse (void **obj, const char *key, const char *value) {
    struct cpu_hardware_info *cpu_info = delegate_cpu->cpu_info;
    struct cpufreq_sys_info *sys_info;
    struct NODE *n = NULL;
    struct ticket_obj *tmp_obj;
    struct cpu_state *tmp_state;
    unsigned long tmp_freq;
    unsigned int cpu_id;

    /* allocate memory for ticket obj */
    if(*obj == NULL) {
        n = node_new(NULL, sizeof(struct ticket_obj));
        if (n == NULL) {
            clog(LOG_ERR, "cannot make enough room for a new ticket OBJ (%s)\n",
                    strerror(errno));
            return -1;
        }
        tmp_obj = (struct ticket_obj *)n->content;
        tmp_obj->private_data = malloc(sizeof(struct cpu_state));;
        if(tmp_obj->private_data != NULL) {
            memset(tmp_obj->private_data, 0, sizeof(struct cpu_state));
            list_append(&delegate_cpu->cpuTicketObj, n);
            *obj = tmp_obj;
        }
        else {
            clog(LOG_ERR, "cannot make enough room for a new cpu_state struct (%s)\n",
                    strerror(errno));
            node_free(n);
            return -1;
        }
    }

    /* parse delegate setting */
    tmp_obj = (struct ticket_obj *)(*obj);
    clog(LOG_DEBUG, "parsing ticket obj 0x%x\n", tmp_obj);
    tmp_state = (struct cpu_state *)(tmp_obj->private_data);
    /* cpu_id must be the first element if has */
    if(strcmp(key,"cpu_id") == 0) {
        if(sscanf(value, "%ul", &cpu_id) != 1) {
            clog(LOG_ERR, "unable to parse <cpu_id> value %s.\n", value);
            return -1;
        }
        if(cpu_id > (cpu_info->cpus-1)) {
            clog(LOG_ERR, "invalid <cpu_id> value %s.\n", value);
            return -1;
        }
        tmp_state->id = cpu_id;
        clog(LOG_DEBUG, "CPU <cpu_id> [%s].\n", value);
        return 0;
    }

    sys_info = cpu_info->sys_info+tmp_state->id;
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
        clog(LOG_DEBUG, "CPU minfreq [%lu].\n", tmp_state->minfreq);
        return 0;
    }

    if(strcmp(key,"maxfreq") == 0) {
        if(sscanf(value, "%lu", &tmp_freq) != 1) {
            clog(LOG_ERR, "unable to parse <minfreq> value %s.\n", value);
            return -1;
        }

        if (strstr(value, "%") != NULL) {
            clog(LOG_DEBUG, "transfer maxfreq percent [%d%].\n", tmp_freq);
            tmp_freq = (sys_info->max) * ((float)tmp_freq / 100);
        }

        tmp_freq = validate_frequency(sys_info, tmp_freq);
        tmp_state->maxfreq = tmp_freq;
        clog(LOG_DEBUG, "CPU maxfreq [%lu].\n", tmp_state->maxfreq);
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
        clog(LOG_DEBUG, "CPU setspeed [%lu].\n", tmp_state->setspeed);
        return 0;
    }

    if (strcmp(key,"governor") == 0) {
        if(0 == validate_governor(sys_info, value)){
            strncpy(tmp_state->governor, value, MAX_GOVERNOR_LEN);
            clog(LOG_DEBUG, "CPU governor [%s].\n", tmp_state->governor);
        }
        return 0;
    }

    if(strcmp(key,"up_threshold") == 0) {
        tmp_state->gov_para.up_threshold = atoi(value);
        clog(LOG_DEBUG, "CPU governor up_threshold [%lu].\n", tmp_state->gov_para.up_threshold);
        return 0;
    }

    if(strcmp(key,"down_threshold") == 0) {
        tmp_state->gov_para.down_threshold = atoi(value);
        clog(LOG_DEBUG, "CPU governor down_threshold [%lu].\n", tmp_state->gov_para.down_threshold);
        return 0;
    }

    if(strcmp(key,"sampling_rate") == 0) {
        tmp_state->gov_para.sampling_rate = atoi(value);
        clog(LOG_DEBUG, "CPU governor sampling_rate [%lu].\n", tmp_state->gov_para.sampling_rate);
        return 0;
    }

    if (strcmp(key,"hotplug") == 0) {
        clog(LOG_DEBUG, "CPU hotplug [%s].\n", value);
        if (strcmp(value, "online") == 0) {
            tmp_state->online = 1;
        } else {
            tmp_state->online = 0;
        }
        clog(LOG_DEBUG, "CPU hotplug [%s]. tmp_state->online %d\n", value, tmp_state->online);
        return 0;
    }

    clog(LOG_DEBUG, "CPU not supported attributes.\n");
    return -1;

}

static int cpu_vote (int valid, unsigned int level, void *obj){
    struct ticket_obj *tmp_obj = (struct ticket_obj *)obj;
    if(tmp_obj->valid != valid) {
        tmp_obj->valid = valid;
        tmp_obj->level = level;
        delegate_need_apply++;
    }
    clog(LOG_DEBUG, "CPU voting obj=0x%x valid=%d level=%d.\n", tmp_obj, tmp_obj->valid, tmp_obj->level);
    return 0;
}

static int cpu_apply (int force) {
    struct ticket_obj *tmp_obj;
    struct cpu_state *tmp_state, *ret_state;
    unsigned int i;

    if (LIST_EMPTY(&delegate_cpu->cpuTicketObj)) {
        clog(LOG_DEBUG, "CPU ticket LIST is empty\n");
        return 0;
    }

    if ((!delegate_need_apply)&&(!force)) {
        clog(LOG_DEBUG, "CPU tickets no change, nothing to apply\n");
        return 0;
    }

    int hotplug_lock_status = 0;//0 stands for no lock, 1 stands for lock.

    for (i=0; i<delegate_cpu->cpu_info->cpus; i++) {
        unsigned int level=0;
        ret_state = delegate_cpu->result+i;
        memset(ret_state, 0, sizeof(struct cpu_state));
        ret_state->id = i;

        /* go through all tickets:
        * replaced by higher level ticket,
        * merge with same level ticket,
        * ignore lower level ticket
        */
        LIST_FOREACH_NODE(node, &delegate_cpu->cpuTicketObj) {
            tmp_obj = (struct ticket_obj*) node->content;
            tmp_state = (struct cpu_state *)tmp_obj->private_data;
            if(tmp_obj->valid && tmp_state->id == i) {
                if(tmp_obj->level > level) {
                    clog(LOG_DEBUG, "CPU Replace ticket private_data data\n");
                    level = tmp_obj->level;
                    ret_state->minfreq = tmp_state->minfreq;
                    ret_state->maxfreq = tmp_state->maxfreq;
                    ret_state->setspeed = tmp_state->setspeed;
                    ret_state->online = tmp_state->online;
                    memset(ret_state->governor, 0, MAX_GOVERNOR_LEN);
                    strcpy(ret_state->governor, tmp_state->governor);
                }

                if(tmp_obj->level == level) {
                    int p,q,r;
                    clog(LOG_DEBUG, "CPU Merge ticket private_data data\n");

                    /* the first time update NULL ret_state */
                    if((ret_state->minfreq == 0) || (ret_state->maxfreq == 0)
                        || ret_state->governor[0] == 0) {
                        clog(LOG_DEBUG, "the first time update NULL ret_state\n");
                        ret_state->minfreq = tmp_state->minfreq;
                        ret_state->maxfreq = tmp_state->maxfreq;
                        ret_state->setspeed = tmp_state->setspeed;
                        memset(ret_state->governor, 0, MAX_GOVERNOR_LEN);
                        strcpy(ret_state->governor, tmp_state->governor);
                        memcpy(&ret_state->gov_para, &tmp_state->gov_para, sizeof(struct governor_parameter));
                        ret_state->online = tmp_state->online;
                        continue;
                    }

                    /* FIXME: Update online first. For online/offline, merge as online*/
                    if(ret_state->online == 0 && tmp_state->online == 1)
                        ret_state->online = tmp_state->online;

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

                    if(!ret_state->governor[0]) {
                        strcpy(ret_state->governor, tmp_state->governor);
                        memcpy(&ret_state->gov_para, &tmp_state->gov_para, sizeof(struct governor_parameter));
                    }
                    else if(strcmp(ret_state->governor, tmp_state->governor)) {
                        for(r = 0; r < PRIO_CPUFREQ_MAX-1; r++)
                            if(!strcmp(ret_state->governor, supported_governors[r]))
                                break;
                        p = r;

                        for(r = 0; r < PRIO_CPUFREQ_MAX-1; r++)
                            if(!strcmp(tmp_state->governor, supported_governors[r]))
                                break;
                        q = r;

                        /* Replace with the governor with a higher priority */
                        if(q > p) {
                            memset(ret_state->governor, 0, MAX_GOVERNOR_LEN);
                            strcpy(ret_state->governor, tmp_state->governor);
                            memcpy(&ret_state->gov_para, &tmp_state->gov_para, sizeof(struct governor_parameter));
                        }
                    }
                    else {
                        /* merge governor parameters, choose higher threshold */
                        if(ret_state->gov_para.up_threshold < tmp_state->gov_para.up_threshold)
                            ret_state->gov_para.up_threshold = tmp_state->gov_para.up_threshold;
                        if(ret_state->gov_para.down_threshold < tmp_state->gov_para.down_threshold)
                            ret_state->gov_para.down_threshold = tmp_state->gov_para.down_threshold;
                        if(ret_state->gov_para.sampling_rate < tmp_state->gov_para.sampling_rate)
                            ret_state->gov_para.sampling_rate = tmp_state->gov_para.sampling_rate;
                    }
                }
            }
        }

        if (ret_state->online && !hotplug_lock_status){
            hotplug_lock_status = 1;
            set_cpu_hotplug_lock(hotplug_lock_status);
        }

        if(hotplug_lock_status)
            set_cpu_online(ret_state);

       /* set cpu frequency */
        set_cpu_frequency(ret_state);
        delegate_need_apply = 0;
    }

    /* set cpu hotplug */
    if (hotplug_lock_status == 0)
        set_cpu_hotplug_lock(hotplug_lock_status);

    return 0;
}


static int cpu_free (void *obj) {

    return 0;
}

static PowerPolicyDaemonDelegateUtils cpu_utils = {
    .get_unit_num       = &get_cpu_num,
    .get_avail_freqs    = &cpu_get_avail_freqs,
    .set_fix_freq       = &cpu_set_freq,
    .get_current_policy = &cpu_get_policy,
};

static PowerPolicyDaemonDelegate cpu = {
    .delegate_name      = "cpu",
    .delegate_init      = &cpu_init,
    .delegate_exit      = &cpu_exit,
    .delegate_parse     = &cpu_parse,
    .delegate_vote      = &cpu_vote,
    .delegate_apply     = &cpu_apply,
    .delegate_free      = &cpu_free,
    .data               = &cpu_utils,
};

PowerPolicyDaemonDelegate *create_delegate (void) {
    return &cpu;
}
