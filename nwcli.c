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
arp_handler(param_t *param, ser_buff_t *tlv_buf, op_mode enable_or_disable)
{
        tlv_struct_t *tlv = NULL;
        char *node_name;
        char *ip_address;

        TLV_LOOP_BEGIN(tlv_buf, tlv)
        {
                if (strncmp(tlv->leaf_id, "node-name", strlen("node-name")) ==
                    0) {
                        node_name = tlv->value;
                } else if (strncmp(tlv->leaf_id, "ip-address",
                                   strlen("ip-address")) == 0) {
                        ip_address = tlv->value;
                }
        }
        TLV_LOOP_END

        printf("node_name: %s and ip address: %s\n", node_name, ip_address);
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
        // param_t *root = libcli_get_root();

        // {
        //         /* show topology */
        //         static param_t topology;
        //         init_param(&topology, CMD, "topology",
        //         show_nw_topology_handler,
        //                    0, INVALID, 0, "Dump complete network topology");
        //         libcli_register_param(show, &topology);
        //         set_param_cmd_code(&topology, CMDCODE_SHOW_NW_TOPOLOGY);
        // }
        {
                /* run node */
                static param_t node;
                init_param(&node, CMD, "node", NULL, NULL, INVALID, NULL,
                           "Help: node");
                libcli_register_param(run, &node);

                {
                        /* run node <node-name: string> */
                        static param_t node_name;
                        init_param(&node_name, LEAF, NULL, NULL, NULL, STRING,
                                   "node-name", "Help: node-name");
                        libcli_register_param(&node, &node_name);
                        {
                                /* run node <node-name: string> resolve-arp */
                                static param_t resolve_arp;
                                init_param(&resolve_arp, CMD, "resolve-arp",
                                           NULL, NULL, INVALID, "resolve_arp",
                                           "Help: resolve_arp");
                                libcli_register_param(&node_name, &resolve_arp);

                                {
                                        /* run node <node-name: string>
                                         * resolve-arp <ip-address> */
                                        static param_t ip_address;
                                        init_param(&ip_address, LEAF,
                                                   "ip-address", arp_handler,
                                                   NULL, IPV4, "ip_address",
                                                   "Help: ip_address");
                                        libcli_register_param(&resolve_arp,
                                                              &ip_address);
                                        set_param_cmd_code(
                                            &ip_address,
                                            CMDCODE_RUN_NODE_IP_ADDRESS);
                                }
                        }
                }
                support_cmd_negation(config);
                start_shell();
        }
}
