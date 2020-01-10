/*
 * Created on Wed Jan 08 2020:14:31:29
 * Created by Ratnadeep Bhattacharya
 */

#include "graph.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned int
hashcode(void *ptr, unsigned int size)
{
        unsigned int val = 0, i = 0;
        char *str = (char *)ptr;
        while (i < size) {
                val += *str;
                val *= 97;
                str++;
                i++;
        }
        return val;
}

void
intf_assign_mac_addr(interface_t *intf)
{
        node_t *node = intf->att_node;
        if (!node) {
                fprintf(stderr, "Attached node not found\n");
                return;
        }
        unsigned int hash_val =
            hashcode((void *)node->node_name, NODE_NAME_SIZE);
        hash_val *= hashcode((void *)intf->if_name, IF_NAME_SIZE);
        memset(IF_MAC(intf), 0, MAC_ADDR_LEN);
        strncpy(IF_MAC(intf), (char *)&hash_val, MAC_ADDR_LEN);
}

bool_t
node_set_lb_addr(node_t *node, char *ip_addr)
{
        assert(ip_addr);
        node->node_nw_prop.is_lb_addr_config = TRUE;
        strncpy(NODE_LO_ADDR(node), ip_addr, IP_ADDR_LEN);

        return TRUE;
}

bool_t
node_set_intf_ip_addr(node_t *node, char *local_if, char *ip_addr, char mask)
{
        interface_t *intf = get_node_if_by_name(node, local_if);
        if (intf == NULL) {
                fprintf(stderr, "Interface not found on this node\n");
                return FALSE;
        }
        // intf_assign_mac_addr(intf); /* mac will be assigned in
        // insert_link_between_two_nodes in graph.c */
        if (intf->intf_nw_props.is_ip_add_config) { /* ip address on this
                                                       interface already set */
                fprintf(stderr, "IP already configured on this interface\n");
                return FALSE;
        }
        intf->intf_nw_props.is_ip_add_config = TRUE;
        strncpy(IF_IP(intf), ip_addr, IP_ADDR_LEN);
        intf->intf_nw_props.mask = mask;
        return TRUE;
}

bool_t
node_unset_intf_ip_addr(node_t *node, char *local_if)
{
        interface_t *intf = get_node_if_by_name(node, local_if);
        if (intf == NULL) {
                fprintf(stderr, "Interface not found on this node");
                return FALSE;
        }
        if (!intf->intf_nw_props.is_ip_add_config) /* IP already deleted */
                return TRUE;
        intf->intf_nw_props.is_ip_add_config = FALSE;
        intf->intf_nw_props.mask = 0;
        memset((void *)IF_IP(intf), 0, IP_ADDR_LEN);
        return TRUE;
}

interface_t *
node_get_matching_subnet_interface(node_t *node, char *ip_addr)
{
        interface_t *intf;
        int i;
        char *local_nw_id;
        char *remote_nw_id;

        local_nw_id = (char *)malloc(sizeof(char) * IP_ADDR_LEN);
        remote_nw_id = (char *)malloc(sizeof(char) * IP_ADDR_LEN);
        intf = NULL;

        for (i = 0; i < MAX_INTF_PER_NODE; i++) {
                char mask = node->intf[i].intf_nw_props.mask;
                apply_mask(IF_IP(&node->intf[i]), mask,
                           local_nw_id); /* get the nw id of the interface */
                apply_mask(ip_addr, mask, remote_nw_id);
                if (strncmp(local_nw_id, remote_nw_id, IP_ADDR_LEN) == 0)
                        intf = &node->intf[i];
        }
        return intf; /* null if none found */
}

void
dump_nw_graph(graph_t *graph)
{
        glthread_t *curr;
        int i;
        curr = NULL;

        ITERATE_GLTHREAD_BEGIN(&graph->node_list, curr)
        {
                node_t *n = thread_to_node(curr);
                printf("Node name: %s\n", n->node_name);
                if (!n->intf) {
                        printf("This node has no interfaces\n");
                } else {
                        if (n->node_nw_prop.is_lb_addr_config)
                                printf("\tLoopback addres: %s\n",
                                       n->node_nw_prop.lb_addr.ip_addr);
                        for (i = 0; i < MAX_INTF_PER_NODE; i++) {
                                if (strncmp(n->intf[i].if_name, "",
                                            IF_NAME_SIZE) != 0) {
                                        // if (&n->intf[i] != NULL) {
                                        printf("\tInterface: %s",
                                               n->intf[i].if_name);
                                        printf(" , nbr node: %s",
                                               n->intf[i].att_node->node_name);
                                        if (n->intf[i]
                                                .intf_nw_props.is_ip_add_config)
                                                printf(", addres: "
                                                       "%s/%d\n",
                                                       n->intf[i]
                                                           .intf_nw_props.ip_add
                                                           .ip_addr,
                                                       n->intf[i]
                                                           .intf_nw_props.mask);
                                }
                        }
                }
        }
        ITERATE_GLTHREAD_END(&graph->node_list, curr);
}

unsigned int
convert_ip_from_str_to_int(char *ip_addr)
{
        char *token;
        char *bits;
        char *delim = ".";
        unsigned int ip;
        int i;

        ip = 0;
        for (i = 0, token = ip_addr;; i++, token = NULL) {
                bits = strtok(token, delim);
                if (!bits)
                        break;
                if (i > 3) {
                        fprintf(stderr, "Wrong ip format\n");
                        return -1;
                }

                *bits *= (unsigned int)pow(256, 3 - i);
                ip += *bits;
        }
        return ip;
}

void
convert_ip_from_int_to_str(unsigned int ip_addr, char *output_buffer)
{
        int i;

        for (i = 0; i < 3; i++) {
                unsigned int num =
                    (int)(ip_addr / (unsigned int)pow(256, 3 - i));
                ip_addr -= num;
                if (i == 0)
                        strcpy(output_buffer, (char *)&num);
                else
                        strcat(output_buffer, (char *)&num);
        }
}