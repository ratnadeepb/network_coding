/*
 * Created on Sat Jun 06 2020:13:56:04
 * Created by Ratnadeep Bhattacharya
 */

#ifdef __COMM__
#define __COMM__

#define MAX_PACKET_BUFFER_SIZE 2048

typedef struct node_ node_t;
typedef struct interface_ interface_t;

*API to send the packet out of the interface.*
        Nbr node must receieve the packet on other end *of the link * /
    int
    send_pkt_out(char *pkt, unsigned int pkt_size, interface_t *interface);

/*API to recv packet from interface*/
int
pkt_receive(node_t *node, interface_t *interface, char *pkt,
            unsigned int pkt_size);

/* API to flood the packet out of all interfaces
 * of the node*/
int
send_pkt_flood(node_t *node, char *pkt, unsigned int pkt_size);

#endif /* __COMM__ */