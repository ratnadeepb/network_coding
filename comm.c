/*
 * Created on Sat Jun 06 2020:13:28:31
 * Created by Ratnadeep Bhattacharya
 */

// #include "comm.h"

#include "graph.h"
#include <arpa/inet.h>
#include <sys/socket.h>

// #include <errno.h>
// #include <memory.h>
// #include <netdb.h>
// #include <netinet/in.h>
// #include <pthread.h>
// #include <stdio.h>
// #include <stdlib.h>

static unsigned int udp_port_number = 40000;

static unsigned int
get_next_udp_port_number()
{
        return udp_port_number++;
}

void
init_udp_socket(node_t *node)
{
        node->udp_port_number = get_next_udp_port_number();
        int udp_sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        struct sockaddr_in node_addr;
        node_addr.sin_family = AF_INET;
        node_addr.sin_port = node->udp_port_number;
        node_addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(udp_sock_fd, (struct sockaddr *)&node_addr,
                 sizeof(struct sockaddr)) == -1) {
                fprintf(stderr, "udp socket bind has failed for node: %s\n",
                        node->node_name);
                return;
        }

        node->udp_sock_fd = udp_sock_fd;
}