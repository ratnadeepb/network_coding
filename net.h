/*
 * Created on Wed Jan 08 2020:13:31:00
 * Created by Ratnadeep Bhattacharya
 */

#ifndef __NET_H__
#define __NET_H__

#include "utils.h"
#include <memory.h>

#define IP_ADDR_LEN 16
#define MAC_ADDR_LEN 8

typedef struct node_ node_t;
typedef struct interface_ interface_t;
typedef struct graph_ graph_t;

typedef struct ip_add_ {
        char ip_addr[IP_ADDR_LEN];
} ip_add_t;

typedef struct mac_add_ {
        char mac[MAC_ADDR_LEN];
} mac_add_t;

typedef struct node_nw_prop_ {
        /* L3 properties */
        bool_t is_lb_addr_config; /* is there a loopback address */
        ip_add_t lb_addr;         /* loopback address of node */
} node_nw_prop_t;

static inline void
init_node_nw_prop(node_nw_prop_t *node_nw_prop)
{
        node_nw_prop->is_lb_addr_config = FALSE;
        memset(node_nw_prop->lb_addr.ip_addr, 0, IP_ADDR_LEN);
}

typedef struct intf_nw_props_ {
        /* L2 properties */
        mac_add_t mac_add;

        /* L3 properties */
        bool_t is_ip_add_config; /* does it have an ip */
        ip_add_t ip_add;
        char mask;
} intf_nw_props_t;

static inline void
init_intf_nw_prop(intf_nw_props_t *intf_nw_prop)
{
        memset(intf_nw_prop->mac_add.mac, 0, MAC_ADDR_LEN);
        intf_nw_prop->is_ip_add_config = FALSE;
        memset(intf_nw_prop->ip_add.ip_addr, 0, IP_ADDR_LEN);
        intf_nw_prop->mask = 0;
}

/* shorthand macros */
#define IF_MAC(intf_ptr) ((intf_ptr)->intf_nw_props.mac_add.mac)
#define IF_IP(intf_ptr) ((intf_ptr)->intf_nw_props.ip_add.ip_addr)

#define NODE_LO_ADDR(node_ptr) (node_ptr->node_nw_prop.lb_addr.ip_addr)
#define IS_INTF_L3_MODE(intf_ptr) ((intf_ptr)->intf_nw_props.is_ip_add_config)

/* set node loopback address */
bool_t
node_set_lb_addr(node_t *node, char *ip_addr);

/* assign a mac to an interface */
void
intf_assign_mac_addr(interface_t *intf);

/* set the ip for an interface for a node */
bool_t
node_set_intf_ip_addr(node_t *node, char *local_if, char *ip_addr, char mask);

/* unset the ip for an interface on the node */
bool_t
node_unset_intf_ip_addr(node_t *node, char *local_if);

/* find a local interface of node that has the same subnet mask, return null if
 * none found */
interface_t *
node_get_matching_subnet_interface(node_t *node, char *ip_addr);

/* print all nodes and interfaces in the topology */
void
dump_nw_graph(graph_t *graph);

/* convert IP from string to int; returns -1 on error */
unsigned int
convert_ip_from_str_to_int(char *ip_addr);

/* convert from number to string */
void
convert_ip_from_int_to_str(unsigned int ip_addr, char *output_buffer);

#endif