/*
 * This is a demo for the CommandParser lib
 * how to configure
 * gcc -g -c main3.c -o main3.o
 * gcc -g main3.o -o exe -L ./CommandParser/ -lcli
 */
#include "CommandParser/cmdtlv.h"
#include "CommandParser/libcli.h"

#define CMDCODE_SHOW_NODE 1
#define CMDCODE_SHOW_NODE_LOOPBACK 2
#define CMDCODE_CONFIG_NODE_LOOPBACK 3

int
node_callback_handler(
    param_t *param, /* pointer to the last cmd node in the complete command */
    ser_buff_t *tlv_buf,        /* shares value passed by the user */
    op_mode enabled_or_disabled /* only valid if it's a config command */
)
{
        printf("%s() is called ...\n", __FUNCTION__);
        return 0;
}

int
validate_node_name(char *value)
{
        printf("%s() is called with value = %s\n", __FUNCTION__, value);
        return VALIDATION_SUCCESS; /* else VALIDATION_FEARLESS */
}

int
node_loopback_callback_handler(param_t *param, ser_buff_t *tlv_buf,
                               op_mode enable_or_disable)
{
        printf("%s() is called ...\n", __FUNCTION__);
        int cmd_code = EXTRACT_CMD_CODE(tlv_buf);
        printf("command code: %d\n", cmd_code);

        tlv_struct_t *tlv = NULL;
        char *node_name;
        char *lo_address;

        TLV_LOOP_BEGIN(tlv_buf, tlv)
        {
                if (strncmp(tlv->leaf_id, "node-name", strlen("node-name")) ==
                    0) {
                        node_name = tlv->value;
                } else if (strncmp(tlv->leaf_id, "lo-adress",
                                   strlen("lo-address")) == 0) {
                        lo_address = tlv->value;
                }
        }
        TLV_LOOP_END

        switch (cmd_code) {
        case CMDCODE_SHOW_NODE_LOOPBACK:
                printf("node name is %s, lo-address is %s\n", node_name,
                       lo_address);
                break;
        case CMDCODE_CONFIG_NODE_LOOPBACK:
                printf("node_name is %s. lo-address is %s, op_code is %s\n",
                       node_name, lo_address,
                       enable_or_disable == CONFIG_ENABLE ? "CONFIG_ENABLE"
                                                          : "CONFIG_DISABLE");
                break;
        default:;
        }
        return 0;
}

int
validate_loopback_address(char *value)
{

        printf("%s() is called with value = %s\n", __FUNCTION__, value);
        return VALIDATION_SUCCESS; /*else return VALIDATION_FAILED*/
}

int
main(int argc, char const *argv[])
{
        init_libcli();
        param_t *show = libcli_get_show_hook();
        param_t *debug = libcli_get_debug_hook();
        param_t *config = libcli_get_config_hook();
        param_t *clear = libcli_get_clear_hook();
        param_t *run = libcli_get_run_hook();

        /* Implementing CMD1: show node <node-name:string> */
        {
                /* show node */
                static param_t node;
                init_param(&node, /* address of the param */
                           CMD, /* CMD for command param, LEAF for leaf param */
                           "node", /* name of the param, string displayed */
                           NULL, /* callback pointer, null here because node is
                                    not a complete command */
                           NULL, /* applicable only on leaf params; callback to
                                    check if string is valid */
                           INVALID, /* always INVALID for CMD */
                           NULL,    /* always null for CMD */
                           "Help: node");
                libcli_register_param(show,
                                      &node); /* add node as a child of show */
                {
                        /* show node <node-name: string> */
                        static param_t node_name;
                        init_param(
                            &node_name,            /* address of param */
                            LEAF,                  /* leaf command */
                            NULL,                  /* always null for leaf */
                            node_callback_handler, /* handle the command */
                            validate_node_name,    /* optional: verify input */
                            STRING,      /* expect a string type value */
                            "node-name", /* give some name to a leaft param */
                            "Help: node-name");
                        libcli_register_param(&node, &node_name);
                        /* every complete command in the command tree should be
                         * associated with a valid command code */
                        set_param_cmd_code(
                            &node_name,
                            CMDCODE_SHOW_NODE); /* assign a unique id to a
                                                   complete command */

                        /* Adding for show node <node-name:string> loopback */
                        {
                                static param_t loopback;
                                init_param(&loopback, CMD, "loopback", NULL,
                                           NULL, INVALID, NULL,
                                           "Help: loopback");
                                libcli_register_param(&node_name, &loopback);

                                /* Adding for show node <node-name:string>
                                 * loopback <loopback-address:string> */
                                static param_t loopback_address;
                                init_param(&loopback_address, LEAF, NULL,
                                           node_loopback_callback_handler,
                                           validate_loopback_address, IPV4,
                                           "lo-address",
                                           "Help: loopback address");
                                libcli_register_param(&loopback,
                                                      &loopback_address);
                                set_param_cmd_code(&loopback_address,
                                                   CMDCODE_SHOW_NODE_LOOPBACK);
                        }
                }
        }

        /* Implementing CMD1: config node <node-name:string> */
        {
                /* show node */
                static param_t node;
                init_param(&node, /* address of the param */
                           CMD, /* CMD for command param, LEAF for leaf param */
                           "node", /* name of the param, string displayed */
                           NULL, /* callback pointer, null here because node is
                                    not a complete command */
                           NULL, /* applicable only on leaf params; callback to
                                    check if string is valid */
                           INVALID, /* always INVALID for CMD */
                           NULL,    /* always null for CMD */
                           "Help: node");
                libcli_register_param(config,
                                      &node); /* add node as a child of show */
                {
                        /* config node <node-name: string> */
                        static param_t node_name;
                        init_param(
                            &node_name,         /* address of param */
                            LEAF,               /* leaf command */
                            NULL,               /* always null for leaf */
                            NULL,               /* handle the command */
                            validate_node_name, /* optional: verify input */
                            STRING,             /* expect a string type value */
                            "node-name", /* give some name to a leaft param */
                            "Help: node-name");
                        libcli_register_param(&node, &node_name);
                        /* every complete command in the command tree should be
                         * associated with a valid command code */

                        /* Adding for config node <node-name:string> loopback */
                        {
                                static param_t loopback;
                                init_param(&loopback, CMD, "loopback", NULL,
                                           NULL, INVALID, NULL,
                                           "Help: loopback");
                                libcli_register_param(&node_name, &loopback);

                                /* Adding for config node <node-name:string>
                                 * loopback <loopback-address:string> */
                                static param_t loopback_address;
                                init_param(&loopback_address, LEAF, NULL,
                                           node_loopback_callback_handler,
                                           validate_loopback_address, IPV4,
                                           "lo-address",
                                           "Help: loopback address");
                                libcli_register_param(&loopback,
                                                      &loopback_address);
                                set_param_cmd_code(
                                    &loopback_address,
                                    CMDCODE_CONFIG_NODE_LOOPBACK);
                        }
                }
        }

        support_cmd_negation(config);
        start_shell();
        return 0;
}
