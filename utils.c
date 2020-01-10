/*
 * Created on Wed Jan 08 2020:15:58:27
 * Created by Ratnadeep Bhattacharya
 */

#include "utils.h"
#include <arpa/inet.h>
#include <stdio.h>

void
apply_mask(char *prefix, char mask, char *str_prefix)
{
        char *dst;
        int i;

        dst = NULL;
        inet_pton(AF_INET, prefix, (void *)dst);
        *dst = htonl(*dst);
        if (mask < 0 || mask > 32) {
                fprintf(stderr, "incorrect mask provided\n");
                return;
        }
        for (i = 0; i < 32 - mask; i++)
                CLEAR_BIT(*dst, i);

        *dst = ntohl(*dst);
        inet_ntop(AF_INET, (void *)dst, str_prefix, 16);
}

void
layer2_fill_with_broadcast_mac(char *mac_array)
{
        mac_array[0] = 0xFF;
        mac_array[1] = 0xFF;
        mac_array[2] = 0xFF;
        mac_array[3] = 0xFF;
        mac_array[4] = 0xFF;
        mac_array[5] = 0xFF;
}