/*
 * Created on Tue Jan 07 2020:14:54:48
 * Created by Ratnadeep Bhattacharya
 */

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "comm.h"
#include "gluethread/glthread.h"
#include "net.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define NODE_NAME_SIZE 16
#define IF_NAME_SIZE 16
#define MAX_INTF_PER_NODE 10

/* forward declarations */
typedef struct graph_ graph_t;
typedef struct node_ node_t;
typedef struct link_ link_t;

typedef struct interface_ {
        char if_name[IF_NAME_SIZE];
        struct node_ *att_node;
        struct link_ *link;
        intf_nw_props_t intf_nw_props;
} interface_t;

struct link_ {
        interface_t intf1;
        interface_t intf2;
        unsigned int cost;
};

struct node_ {
        char node_name[NODE_NAME_SIZE];
        interface_t intf[MAX_INTF_PER_NODE];
        glthread_t graph_glue;
        unsigned int udp_port_number;
        int udp_sock_fd;
        node_nw_prop_t node_nw_prop;
};

struct graph_ {
        char topology_name[32];
        glthread_t node_list;
};

static inline node_t *
get_nbr_node(interface_t *interface)
{
        assert(interface->att_node);
        assert(interface->link);

        link_t *link = interface->link;

        if (&link->intf1 == interface)
                return link->intf2.att_node;
        else
                return link->intf1.att_node;
}

static inline int
get_node_intf_available_slot(node_t *node)
{
        int i;
        for (i = 0; i < MAX_INTF_PER_NODE; i++) {
                if (strncmp(node->intf[i].if_name, "", IF_NAME_SIZE) == 0)
                        return i;
        }
        return -1;
}

/* return local interface of a node */
static inline interface_t *
get_node_if_by_name(node_t *node, char *if_name)
{
        int i;

        if (!node->intf) /* interface array is empty */
                return NULL;
        for (i = 0; i < MAX_INTF_PER_NODE; i++) {
                if (&node->intf[i] != NULL)
                        if (strncmp(node->intf[i].if_name, if_name,
                                    IF_NAME_SIZE) == 0) {
                                return &node->intf[i];
                        }
        }
        return NULL; /* if interface not found */
}

GLTHREAD_TO_STRUCT(thread_to_node, node_t, graph_glue);

/* return node, if present, by name from graph */
static inline node_t *
get_node_by_node_name(graph_t *first_graph, char *node_name)
{
        glthread_t *glthreadptr = NULL;
        /* if the graph is empty return null */
        if (IS_GLTHREAD_LIST_EMPTY(&first_graph->node_list))
                return NULL;

        ITERATE_GLTHREAD_BEGIN(&first_graph->node_list, glthreadptr)
        {
                node_t *n = thread_to_node(glthreadptr);
                if (strncmp(n->node_name, node_name, NODE_NAME_SIZE) == 0)
                        return n;
        }
        ITERATE_GLTHREAD_END(&first_graph->node_list, glthreadptr);
        return NULL; /* if node not found */
}

/* create a new graph */
graph_t *
create_new_graph(char *topology_name);

/* create a new node and add it to a graph */
node_t *
create_graph_node(graph_t *graph, char *node_name);

/* insert a link between two nodes */
void
insert_link_between_two_nodes(node_t *node1, node_t *node2, char *from_if_name,
                              char *to_if_name, unsigned int cost);

/* display routine */
void
dump_graph(__attribute__((unused)) graph_t *graph);

#endif