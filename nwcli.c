/*
 * Created on Mon Jan 27 2020:21:18:54
 * Created by Ratnadeep Bhattacharya
 */

#include "CommandParser/cmdtlv.h"
#include "CommandParser/libcli.h"
#include "cmdcodes.h"
#include "graph.h"
#include <stdio.h>

extern graph_t *first_graph;

/* generic topology commands */
static int
show_nw_topology_handler(param_t *param, ser_buff_t *tlv_buff,
                         op_mode enable_or_disable)
{
        int CMDCODE = -1;
        CMDCODE = EXTRACT_CMD_CODE(tlv_buff);

        switch (CMDCODE) {
        case CMDCODE_SHOW_NW_TOPOLOGY:
                dump_nw_graph(first_graph);
                break;

        default:
                break;
        }
        return 0;
}

static int
arp_handler(param_t *param, ser_buff_t *tlv_buff, op_mode enable_or_disable)
{
        tlv_struct_t *tlv = NULL;
        char *node_name = NULL;
        char *mac = NULL;
        int CMDCODE;

        CMDCODE = CMD_SHOW_NODE_IP_ARP;

        TLV_LOOP_BEGIN(tlv_buff, tlv)
        {
                if (strncmp(tlv->leaf_id, "node", strlen("node")) == 0) {
                        node_name = tlv->value;
                } else if (strncmp(tlv->leaf_id, "resolve-arp",
                                   strlen("resolve-arp")) == 0) {
                        mac = tlv->value;
                }
        }
        TLV_LOOP_END;

        switch (CMDCODE) {
        case CMD_SHOW_NODE_IP_ARP:
                printf("node name: %s. Resolved MAC: %s\n", node_name, mac);
                break;

        default:
                break;
        }

        return 0;
}

void
nw_init_cli()
{
        init_libcli();

        param_t *show = libcli_get_show_hook();
        param_t *debug = libcli_get_debug_hook();
        param_t *config = libcli_get_config_hook();
        param_t *run = libcli_get_run_hook();
        param_t *debug_show = libcli_get_debug_show_hook();
        param_t *root = libcli_get_root();

        // {
        //         /* show topology */
        //         static param_t topology;
        //         init_param(&topology, CMD, "topology",
        //         show_nw_topology_handler,
        //                    0, INVALID, 0, "Dump complete network topology");
        //         libcli_register_param(show, &topology);
        //         set_param_cmd_code(&topology, CMDCODE_SHOW_NW_TOPOLOGY);

        // 	/* resolve arp */
        // 	static param_t mac;
        // 	init_param(&mac, LEAF, "ARP resolution", arp_handler, 0, IPV4,
        // "resolve-arp", "Resolve arp for the node IP");
        // 	libcli_register_param()
        // }
        support_cmd_negation(config);
}
