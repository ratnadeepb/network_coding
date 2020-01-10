/*
 * Created on Tue Jan 07 2020:21:49:24
 * Created by Ratnadeep Bhattacharya
 */

#include "graph.h"
#include <stdlib.h>

graph_t *
create_new_graph(char *topology_name)
{
        glthread_t base_glthread;
        graph_t *graph;

        init_glthread(&base_glthread);

        graph = (graph_t *)malloc(sizeof(graph_t));

        if (!graph)
                return NULL;

        strcpy(graph->topology_name, topology_name);
        graph->node_list = base_glthread;
        return graph;
}

node_t *
create_graph_node(graph_t *graph, char *node_name)
{
        node_t *node = NULL;
        node = (node_t *)malloc(sizeof(node_t));

        if (!node)
                return NULL;

        strcpy(node->node_name, node_name);
        init_node_nw_prop(&node->node_nw_prop);
        glthread_add_next(&graph->node_list, &node->graph_glue);
        return node;
}

void
insert_link_between_two_nodes(node_t *node1, node_t *node2, char *from_if_name,
                              char *to_if_name, unsigned int cost)
{
        link_t *link;
        link = calloc(1, sizeof(link_t));

        /* set interface properties */
        strncpy(link->intf1.if_name, from_if_name, IF_NAME_SIZE);
        strncpy(link->intf2.if_name, to_if_name, IF_NAME_SIZE);

        link->intf1.link = link;
        link->intf2.link = link;

        link->intf1.att_node = node1;
        link->intf2.att_node = node2;

        init_intf_nw_prop(&link->intf1.intf_nw_props);
        init_intf_nw_prop(&link->intf2.intf_nw_props);

        intf_assign_mac_addr(&link->intf1);
        intf_assign_mac_addr(&link->intf2);

        link->cost = cost;

        int intf_slot;
        intf_slot = get_node_intf_available_slot(node1);
        if (intf_slot > -1)
                node1->intf[intf_slot] = link->intf1;
        intf_slot = get_node_intf_available_slot(node2);
        if (intf_slot > -1)
                node2->intf[intf_slot] = link->intf2;
}

/* display routine */
void
dump_graph(__attribute__((unused)) graph_t *graph)
{
}