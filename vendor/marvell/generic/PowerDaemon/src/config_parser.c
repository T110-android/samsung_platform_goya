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

#include <ctype.h>
#include <errno.h>
#include <grp.h>
#include <limits.h>
#include <sys/types.h>
#include <string.h>
#include "config_parser.h"
#include "ppd_log.h"
#include "ppd_plugin_utils.h"
#include "ppd_delegate_utils.h"

/*
 * parse the <General> section
 *
 * Returns -1 if required properties are missing, 0 otherwise
 */
static int parse_config_general (xmlNodePtr pXmlNode, PowerPolicyDaemonConf *config) {
	xmlNodePtr cur_node = pXmlNode->xmlChildrenNode;
	int found;

	if(cur_node == NULL)
		return -1;

	/* parse poll_interval tag */
	for (found=0,cur_node = pXmlNode->xmlChildrenNode;
			cur_node != NULL; cur_node=cur_node->next) {
		if (xmlStrcmp(cur_node->name, (const xmlChar *)"poll_interval") == 0) {
			xmlChar *interval = xmlNodeListGetString(config->doc, cur_node->xmlChildrenNode, 1);
			clog(LOG_INFO, "PPD <poll_interval> %s", interval);

			unsigned int poll_val = (unsigned int)atoi((const char *)interval);
			config->poll_intv.tv_usec = 0;
			config->poll_intv.tv_sec = poll_val;
			xmlFree(interval);

			clog(LOG_DEBUG, "poll_interval is %lu.%lu seconds\n",
					config->poll_intv.tv_sec,
					config->poll_intv.tv_usec);
			found = 1;
			break;
		}
	}
	if(!found) {
		config->poll_intv.tv_usec = 0;
		config->poll_intv.tv_sec = DEFAULT_POLL;
		clog(LOG_DEBUG, "set default poll_interval %lu.%lu seconds\n",
				config->poll_intv.tv_sec,
				config->poll_intv.tv_usec);
	}

	/* parse verbosity tag */
	for (found=0,cur_node = pXmlNode->xmlChildrenNode;
			cur_node != NULL; cur_node=cur_node->next) {
		if (xmlStrcmp(cur_node->name, (const xmlChar *)"verbosity") == 0) {
			xmlChar *verbosity = xmlNodeListGetString(config->doc, cur_node->xmlChildrenNode, 1);
			clog(LOG_INFO, "PPD <verbosity> %s", verbosity);

			config->log_level = (int)atoi((const char *)verbosity);
			xmlFree(verbosity);
			clog(LOG_DEBUG, "PPD verbosity is %d\n",config->log_level);
			found = 1;
			break;
		}
	}
	if(!found) {
		config->log_level = DEFAULT_VERBOSITY;
		clog(LOG_DEBUG, "set default verbosity %d\n",config->log_level);
	}

	/* parse enable_remote tag */
	for (found=0,cur_node = pXmlNode->xmlChildrenNode;
			cur_node != NULL; cur_node=cur_node->next) {
		if (xmlStrcmp(cur_node->name, (const xmlChar *)"enable_remote") == 0) {
			xmlChar *remote = xmlNodeListGetString(config->doc, cur_node->xmlChildrenNode, 1);
			clog(LOG_INFO, "PPD <enable_remote> %s", remote);

			config->enable_remote = (unsigned int)atoi((const char *)remote);
			xmlFree(remote);
			found = 1;
			clog(LOG_DEBUG, "PPD enable_remote is %d\n",config->enable_remote);
			break;
		}
	}
	if(!found) {
		config->enable_remote = 1;
		clog(LOG_DEBUG, "set default enable_remote %d\n",config->enable_remote);
	}

	/* parse remote_group tag */
	for (found=0,cur_node = pXmlNode->xmlChildrenNode;
			cur_node != NULL; cur_node=cur_node->next) {
		if (xmlStrcmp(cur_node->name, (const xmlChar *)"remote_group") == 0) {
			xmlChar *group = xmlNodeListGetString(config->doc, cur_node->xmlChildrenNode, 1);
			clog(LOG_INFO, "PPD <remote_group> %s", group);

			struct group *grp = NULL;
			char *token;
			long int gid = strtol((char *)group, &token, 10);

			/* in case value doesn't hold a number or that
			* number is not acceptable try to consider it a
			* groupname otherwise validate the given gid.
			* If it still fails, shout!
			* Note: the group_id limit is pretty arbitrary here...
			*/
			if ((*token == '\0' && gid > 0 && (unsigned long int)gid < USHRT_MAX && (grp = getgrgid((gid_t)gid)) != NULL) || (grp = getgrnam((char *)group)) != NULL) {
				config->remote_gid = grp->gr_gid;
				clog(LOG_WARNING, "Remote controls will be r/w for group %s (%d).\n", grp->gr_name, grp->gr_gid);
			} else {
				config->remote_gid = 0;
				clog(LOG_WARNING, "remote_group contains an invalid value (%s), r/w group permissions will remain unchanged.\n", group);
			}

			xmlFree(group);
			found = 1;
			clog(LOG_DEBUG, "PPD remote_group is %d\n",config->remote_gid);
			break;
		}
	}
	if(!found) {
		config->remote_gid = 0;
		clog(LOG_ERR, "set default remote_group %d\n",config->remote_gid);
	}

	return 0;
}

/*
 * parse the <Plugins> section
 *
 * Returns -1 if required properties are missing, 0 otherwise
 */
static int parse_config_plugins (xmlNodePtr pXmlNode, PowerPolicyDaemonConf *config) {
	struct plugin_obj *o_plugin = NULL;
	xmlNodePtr cur_node = pXmlNode->xmlChildrenNode;
	xmlNodePtr subnode;
	
	clog(LOG_INFO, "parse Plugins\n");

	if(cur_node == NULL)
		return -1;

	while(cur_node){
		if(cur_node->type != XML_ELEMENT_NODE) {
			cur_node = cur_node->next;
			continue;
		}

		/* foreach plugin */
		LIST_FOREACH_NODE(node, &config->plugins) {
			o_plugin = (struct plugin_obj *)node->content;
			if (o_plugin == NULL || o_plugin->plugin == NULL)
				continue;
		
			if (strcmp((const char *)(cur_node->name), o_plugin->plugin->plugin_name) == 0) {
				clog(LOG_INFO, "Found Section for <%s>\n",
						o_plugin->plugin->plugin_name);

				/* enable attribute */
				xmlChar *enable = xmlGetProp(cur_node, (const xmlChar *)"enable");
				if(enable && atoi((const char *)enable)){
					o_plugin->used = 1;
				}
				xmlFree(enable);

				if(o_plugin->used && o_plugin->plugin->plugin_conf != NULL){
					subnode = cur_node->xmlChildrenNode;
					while(subnode){
						if(subnode->type != XML_ELEMENT_NODE) {
							subnode = subnode->next;
							continue;
						}

						const char *name = (const char *)subnode->name;
						xmlChar *value = xmlNodeListGetString(config->doc, subnode->xmlChildrenNode, 1);
						clog(LOG_INFO, "plugin %s config <%s>=%s", 
							o_plugin->plugin->plugin_name, name, (const char *)value);
						o_plugin->plugin->plugin_conf(name, (const char *)value);
						xmlFree(value);
						subnode = subnode->next;
					}
					clog(LOG_DEBUG, "<===== plugin %s parse completed =====>",o_plugin->plugin->plugin_name);
				}
			}
		}
		cur_node = cur_node->next;
	}

	return 0;
}

/*
 * parse [constraint ticket] section
 *
 * Returns -1 if required properties are missing, 0 otherwise
 */
static int parse_constraint_ticket (PowerPolicyDaemonConstraint *constraint, xmlNodePtr pXmlNode, PowerPolicyDaemonConf *config) {
	xmlNodePtr cur_node = pXmlNode->xmlChildrenNode;
	xmlNodePtr subnode;
	struct delegate_obj *o_delegate = NULL;
	struct NODE *n = NULL;
	struct ticket *tmp_ticket = NULL;
	int found_delegate = 0;

	if(cur_node == NULL)
		return -1;

	while(cur_node){
		if(cur_node->type != XML_ELEMENT_NODE) {
			cur_node = cur_node->next;
			continue;
		}

		n = node_new(NULL, sizeof(struct ticket));
		if (n == NULL) {
			clog(LOG_ERR, "cannot make enough room for a new ticket (%s)\n",
					strerror(errno));
			return -1;
		}
		tmp_ticket = (struct ticket *)n->content;

		found_delegate = 0;
		/* find related delegate */
		LIST_FOREACH_NODE(node, &config->delegates) {
			o_delegate = (struct delegate_obj *)node->content;
			if (strcmp(o_delegate->delegate->delegate_name, (const char *)(cur_node->name)) != 0)
				continue;

			clog(LOG_INFO, "Found Delegate Section <%s>\n", o_delegate->delegate->delegate_name);
			found_delegate = 1;
			tmp_ticket->delegate = o_delegate->delegate;
			o_delegate->used = 1;
			break;
		}

		if(!found_delegate) {
			clog(LOG_WARNING, "Not Found Delegate Section <%s>\n", (const char *)(cur_node->name));
			node_free(n);
			cur_node = cur_node->next;
			continue;
		}

		if(o_delegate->delegate->delegate_parse != NULL) {
			/* delegate keywords */
			subnode = cur_node->xmlChildrenNode;
			while(subnode) {
				if(subnode->type != XML_ELEMENT_NODE) {
					subnode = subnode->next;
					continue;
				}

				const char *name = (const char *)subnode->name;
				xmlChar *value = xmlNodeListGetString(config->doc, subnode->xmlChildrenNode, 1);
				clog(LOG_INFO, "delegate %s keyword <%s>=%s", 
					o_delegate->delegate->delegate_name, name, (const char *)value);
				o_delegate->delegate->delegate_parse(&tmp_ticket->obj, name, (const char *)value);
				xmlFree(value);
				subnode = subnode->next;
			}

			if(tmp_ticket->obj)
				list_append(&constraint->tickets, n);
			else
				node_free(n);
			
		}
		else {
			node_free(n);
		}

		cur_node = cur_node->next;
	}

	if (LIST_EMPTY(&constraint->tickets)) {
		return -1;
	}
	return 0;
}
/*
 * parse <Constraints> section
 *
 * Returns -1 if required properties are missing, 0 otherwise
 */
static int parse_config_constraints (xmlNodePtr pXmlNode, PowerPolicyDaemonConf *config) {
	xmlNodePtr cur_node = pXmlNode->xmlChildrenNode;
	struct NODE *n = NULL;
	PowerPolicyDaemonConstraint *tmp_constraint = NULL;
	PowerPolicyDaemonRule *tmp_rule = NULL;
	const char *constraint_name;

	clog(LOG_INFO, "parse Constraints\n");

	if(cur_node == NULL)
		return -1;

	while(cur_node){
		if(cur_node->type != XML_ELEMENT_NODE) {
			cur_node = cur_node->next;
			continue;
		}

		constraint_name = (const char *)(cur_node->name);
		clog(LOG_DEBUG, "<======== parsing constraint [%s] ========>\n", constraint_name);

		/* find all rules relate to this constraint */
		LIST_FOREACH_NODE(node, &config->rules) {
			tmp_rule = (PowerPolicyDaemonRule *)node->content;
			if ((tmp_rule->constraint != NULL)||(strcmp(tmp_rule->constraint_name, constraint_name) != 0))
				continue;

			clog(LOG_INFO, "Rule [%s] found related Constraint <%s>\n", tmp_rule->rule_name, constraint_name);

			n = node_new(NULL, sizeof(PowerPolicyDaemonConstraint));
			if (n == NULL) {
				clog(LOG_ERR, "cannot make enough room for a new Constraint (%s)\n",
						strerror(errno));
				return -1;
			}
			tmp_constraint = (PowerPolicyDaemonConstraint *)n->content;

			strcpy(tmp_constraint->name, constraint_name);
			pthread_mutex_lock(&config->ppd_mutex);
			tmp_constraint->active = 0;
			pthread_mutex_unlock(&config->ppd_mutex);
			if(parse_constraint_ticket(tmp_constraint, cur_node, config) < 0) {
				node_free(n);
				n = NULL;
			}
			if (n != NULL) {
				/* reserved ptr each other */
				tmp_constraint->rule = tmp_rule;
				tmp_rule->constraint = tmp_constraint;
				list_append(&config->constraints, n);
			}
		}
		cur_node = cur_node->next;
	}

	return 0;
}

/*
 * parse [rule directive] section
 *
 * Returns -1 if required properties are missing, 0 otherwise
 */
static int parse_rule_directive(PowerPolicyDaemonRule *rule, xmlNodePtr pXmlNode, PowerPolicyDaemonConf *config) {
	xmlNodePtr cur_node = pXmlNode->xmlChildrenNode;
	struct NODE *dir = NULL;
	void *obj = NULL; /* to hold the value provided by a plugin */
	PowerPolicyDaemonKeyword *ckw = NULL;
	PowerPolicyDaemonPlugin *plugin = NULL;

	if(cur_node == NULL)
		return -1;

	while(cur_node){
		if(cur_node->type != XML_ELEMENT_NODE) {
			cur_node = cur_node->next;
			continue;
		}

		const char *name = (const char *)cur_node->name;
		xmlChar *value = xmlNodeListGetString(config->doc, cur_node->xmlChildrenNode, 1);
		clog(LOG_INFO, "rule %s directive <%s> = %s", rule->rule_name, name, (const char *)value);

		/* it's plugin time to tell if they like the directive */
		ckw = plugin_handle_keyword(&config->plugins, name, (const char *)value, &obj, &plugin);
		/* if plugin found append to the list */
		if (ckw) {
			dir = node_new(NULL, sizeof(struct directive));
			if (dir == NULL) {
				free_keyword_object(ckw, obj);
				clog(LOG_ERR, "cannot make enough room for a new directive (%s).\n",
						strerror(errno));
				return -1;
			}
			((struct directive *)dir->content)->keyword = ckw;
			((struct directive *)dir->content)->obj = obj;
			((struct directive *)dir->content)->plugin = plugin;
			/* ok, append the rule entry */
			list_append(&(rule->directives), dir);
			rule->directives_count++;
		}
		xmlFree(value);

		cur_node = cur_node->next;
	}
	return 0;
}

/*
 * parses a <Rules> section
 *
 * Returns -1 if required properties are missing, 0 otherwise
 */
static int parse_config_rules (xmlNodePtr pXmlNode, PowerPolicyDaemonConf *config) {
	xmlNodePtr cur_node = pXmlNode->xmlChildrenNode;
	struct NODE *n = NULL;
	PowerPolicyDaemonRule *tmp_rule = NULL;

	clog(LOG_INFO, "parse Rules\n");

	if(cur_node == NULL)
		return -1;

	while(cur_node){
		if(cur_node->type != XML_ELEMENT_NODE) {
			cur_node = cur_node->next;
			continue;
		}

		n = node_new(NULL, sizeof(PowerPolicyDaemonRule));
		if (n == NULL) {
			clog(LOG_ERR, "cannot make enough room for a new Rule (%s)\n",
					strerror(errno));
			return -1;
		}
		tmp_rule = (PowerPolicyDaemonRule *)n->content;

		/* rule name */
		strcpy(tmp_rule->rule_name, (const char *)(cur_node->name));
		clog(LOG_DEBUG, "<======== parsing rule [%s] ========>\n", tmp_rule->rule_name);

		/* level attribute */
		xmlChar *level = xmlGetProp(cur_node, (const xmlChar *)"level");
		if(level){
			tmp_rule->level = (unsigned int)atoi((const char *)level);
		}
		clog(LOG_DEBUG, "Rule [%s] level = %d\n", tmp_rule->rule_name, tmp_rule->level);
		xmlFree(level);

		/* constraint name attribute */
		xmlChar *constraint_name = xmlGetProp(cur_node, (const xmlChar *)"constraint");
		if(constraint_name){
			strcpy(tmp_rule->constraint_name, (const char *)(constraint_name));
		}
		clog(LOG_DEBUG, "Rule [%s] relate constraint = %s\n", tmp_rule->rule_name, tmp_rule->constraint_name);
		xmlFree(constraint_name);

		/* enable attribute */
		xmlChar *enable = xmlGetProp(cur_node, (const xmlChar *)"enable");
		if(enable && atoi((const char *)enable)){
			tmp_rule->enable = 1;
		}
		clog(LOG_DEBUG, "Rule [%s] enable = %d\n", tmp_rule->rule_name, tmp_rule->enable);
		xmlFree(enable);

		if(!tmp_rule->enable) {
			node_free(n);
			n = NULL;
			clog(LOG_INFO, "Rule [%s] disabled.\n", tmp_rule->rule_name);

			cur_node = cur_node->next;
			continue;
		}

		parse_rule_directive(tmp_rule, cur_node, config);

		/* check if there are options */
		if (LIST_EMPTY(&tmp_rule->directives)) {
			clog(LOG_CRIT, "Rule [%s] has no options. Discarded.\n",
					tmp_rule->rule_name);
			node_free(n);
			n = NULL;
			cur_node = cur_node->next;
			continue;
		}

		if (n != NULL)
			list_append(&config->rules, n);

		cur_node = cur_node->next;
	}



	/*
	 * associate rules->constraints
	 * go through rules and associate to the proper profile
	 */
	return 0;
}

/* intialize the PowerPolicyDaemonConf object
 * by reading the configuration file
 */
int init_configuration(PowerPolicyDaemonConf *config)
{
	xmlNodePtr cur_node;

	/* read and initialize plugins */
	discover_plugins(&config->plugins);
	load_plugin_list(&config->plugins);

	/* read and initialize delegates */
	discover_delegates(&config->delegates);
	load_delegate_list(&config->delegates);

	/* configuration file */
	clog(LOG_INFO, "reading configuration file %s\n", config->config_file);
	config->doc = xmlParseFile(config->config_file);
	if (config->doc == NULL) {
		clog(LOG_ERR, "failed to parse xml file: %s\n", config->config_file);
		return -1;
	}

	/* start xml parse */
	cur_node = xmlDocGetRootElement(config->doc);
	if (cur_node == NULL) {
		clog(LOG_ERR, "failed to get root element in %s\n", config->config_file);
		return -1;
	}

	if (xmlStrcmp(cur_node->name, (const xmlChar *)"MarvellPowerPolicyDaemon")) {
		clog(LOG_ERR, "wrong config file, root node != MarvellPowerPolicyDaemon");
		xmlFreeDoc(config->doc);
		return -1;
	}

	/* now scan the tree */
	cur_node = cur_node->xmlChildrenNode;
	while(cur_node && xmlIsBlankNode(cur_node)) {
		cur_node = cur_node->next;
	}

	if(cur_node == NULL) {
		xmlFreeDoc(config->doc);
		return -1;
	}

	while (cur_node) {
		if(cur_node->type != XML_ELEMENT_NODE) {
			cur_node = cur_node->next;
			continue;
		}

		/* parse version tag */
		if (xmlStrcmp(cur_node->name, (const xmlChar *)"Version") == 0) {
			xmlChar *version = xmlNodeListGetString(config->doc, cur_node->xmlChildrenNode, 1);
			clog(LOG_INFO, "PPD <Version> %s", version);

			config->version = ((atoi(strtok((char *)version, "."))) | (atoi(strtok(NULL, ".")) << 8));
			xmlFree(version);
		}

		/* parse General tag */
		if (xmlStrcmp(cur_node->name, (const xmlChar *)"General") == 0) {
			if (parse_config_general(cur_node, config) < 0) {
				clog(LOG_ERR, "PPD parse_config_general err!");
				xmlFreeDoc(config->doc);
				return -1;
			}
		}

		/* parse Plugins tag */
		if (xmlStrcmp(cur_node->name, (const xmlChar *)"Plugins") == 0) {
			if (parse_config_plugins(cur_node, config) < 0) {
				clog(LOG_ERR, "PPD parse_config_plugins err!");
				xmlFreeDoc(config->doc);
				return -1;
			}
		}

		/* Plugins post config */
		plugins_post_conf(&config->plugins);

		/* parse Rules tag */
		if (xmlStrcmp(cur_node->name, (const xmlChar *)"Rules") == 0) {
			if (parse_config_rules(cur_node, config) < 0) {
				clog(LOG_ERR, "PPD parse_config_rules err!");
				xmlFreeDoc(config->doc);
				return -1;
			}
		}

		/* parse Constraints tag */
		if (xmlStrcmp(cur_node->name, (const xmlChar *)"Constraints") == 0) {
			if (parse_config_constraints(cur_node, config) < 0) {
				clog(LOG_ERR, "PPD parse_config_constraints err!");
				xmlFreeDoc(config->doc);
				return -1;
			}
		}

		cur_node = cur_node->next;
	} /* end while */
	xmlFreeDoc(config->doc);

	/* did I read something?
	 * check if I read at least one rule, otherwise exit
	 */
	if (LIST_EMPTY(&config->rules)) {
		clog(LOG_ERR, "ERROR! No rules found!\n");
		return -1;
	}

	return 0;
}

/*
 * Frees the structures allocated.
 */
void free_configuration(PowerPolicyDaemonConf *config)
{
	PowerPolicyDaemonRule *tmp_rule;
	PowerPolicyDaemonConstraint *tmp_constraint;
	struct directive *tmp_directive;
	struct plugin_obj *o_plugin;
	struct delegate_obj *o_delegate;

	/* cleanup rule directives and profile arrays */
	if (LIST_EMPTY(&config->rules))
		goto clean_constraints;
	clog(LOG_INFO, "freeing rules directives.\n");
	LIST_FOREACH_NODE(node, &config->rules) {
		tmp_rule = (PowerPolicyDaemonRule *) node->content;

		LIST_FOREACH_NODE(node1, &tmp_rule->directives) {
			tmp_directive = (struct directive *) node1->content;
			free_keyword_object(tmp_directive->keyword, tmp_directive->obj);
		}
		list_free_sublist(&tmp_rule->directives, tmp_rule->directives.first);
	}
	/* cleanup rule structs */
	clog(LOG_INFO, "freeing rules.\n");
	list_free_sublist(&(config->rules), config->rules.first);
	config->rules.first = config->rules.last = NULL;

clean_constraints:
	/* cleanup constraint tickets */
	if (LIST_EMPTY(&config->constraints))
		goto clean_plugins;
	clog(LOG_INFO, "freeing constraint tickets.\n");
	LIST_FOREACH_NODE(node, &config->constraints) {
		tmp_constraint = (PowerPolicyDaemonConstraint *) node->content;
		if (LIST_EMPTY(&tmp_constraint->tickets))
			continue;
		list_free_sublist(&tmp_constraint->tickets, tmp_constraint->tickets.first);
	}
	clog(LOG_INFO, "freeing constraints.\n");
	list_free_sublist(&(config->constraints), config->constraints.first);
	config->constraints.first = config->constraints.last = NULL;

clean_plugins:
	/* finalize plugins!!!! */
	/*
	 *  Unload plugins
	 */
	if (LIST_EMPTY(&config->plugins))
		goto clean_delegates;
	clog(LOG_INFO, "freeing plugins.\n");
	LIST_FOREACH_NODE(node, &config->plugins) {
		o_plugin = (struct plugin_obj*) node->content;
		finalize_plugin(o_plugin);
		close_plugin(o_plugin);
	}
	list_free_sublist(&(config->plugins), config->plugins.first);
	config->plugins.first = config->plugins.last = NULL;

clean_delegates:
	/* finalize delegates!!!! */
	/*
	 *	Unload delegates
	 */
	if (LIST_EMPTY(&config->delegates))
		goto clean_other;
	clog(LOG_INFO, "freeing delegates.\n");
	LIST_FOREACH_NODE(node, &config->delegates) {
		o_delegate = (struct delegate_obj*) node->content;
		finalize_delegate(o_delegate);
		close_delegate(o_delegate);
	}
	list_free_sublist(&(config->delegates), config->delegates.first);
	config->delegates.first = config->delegates.last = NULL;

clean_other:
	/* clean other values */
	config->poll_intv.tv_usec = 0;
	config->poll_intv.tv_sec = DEFAULT_POLL;
	config->has_sysfs = 0;
	config->enable_remote = 0;

	pthread_mutex_destroy(&config->ppd_mutex);

}
