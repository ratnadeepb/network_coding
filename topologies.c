/*
 * Created on Tue Jan 07 2020:21:34:38
 * Created by Ratnadeep Bhattacharya
 */

#include "graph.h"

graph_t *
build_first_topo()
{
#if 0

                          +----------+
                      0/4 |          |0/0
         +----------------+   R0_re  +---------------------------+
         |     40.1.1.1/24| 122.1.1.0|20.1.1.1/24                |
         |                +----------+                           |
         |                                                       |
         |                                                       |
         |                                                       |
         |40.1.1.2/24                                            |20.1.1.2/24
         |0/5                                                    |0/1
     +---+---+                                              +----+-----+
     |       |0/3                                        0/2|          |
     | R2_re +----------------------------------------------+    R1_re |
     |       |30.1.1.2/24                        30.1.1.1/24|          |
     +-------+                                              +----------+

#endif

        /* create new graph */
        graph_t *topo = create_new_graph("Hello World generic graph");

        /* create new nodes */
        node_t *R0_re = create_graph_node(topo, "R0_re");
        node_t *R1_re = create_graph_node(topo, "R1_re");
        node_t *R2_re = create_graph_node(topo, "R2_re");

        /* insert links between nodes */
        insert_link_between_two_nodes(R0_re, R1_re, "eth0/0", "eth0/1",
                                      1); /* R0 -> R1 */
        insert_link_between_two_nodes(R1_re, R2_re, "eth0/2", "eth0/3",
                                      1); /* R1 -> R2 */
        insert_link_between_two_nodes(R0_re, R2_re, "eth0/4", "eth0/5",
                                      1); /* R0 -> R2 */

        return topo;
}