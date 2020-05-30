/*
 * Created on Mon Jan 27 2020:21:35:49
 * Created by Ratnadeep Bhattacharya
 */

#include "CommandParser/libcli.h"
#include "graph.h"
#include <stdio.h>

extern graph_t *
build_first_topo();
extern void
nw_init_cli();

graph_t *first_graph = NULL;

int
main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv)
{
        // __attribute__((unused)) graph_t *first_graph;
        first_graph = build_first_topo();
        nw_init_cli();
        // dump_nw_graph(first_graph);
        // start_shell();
        return 0;
}