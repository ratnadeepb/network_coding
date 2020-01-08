/*
 * Created on Wed Jan 08 2020:13:35:05
 * Created by Ratnadeep Bhattacharya
 */

#ifndef __UTILS_H__
#define __UTILS_H__

typedef enum {
        FALSE,
        TRUE,
} bool_t;

/* prefix represents and IP address
 * given IP and mask fill string representation of mask */
void
apply_mask(char *prefix, char mask, char *str_prefix);

/* fill broadcast MAC address */
void
layer2_fill_with_broadcast_mac(char *mac_array);

#define SET_BIT(value, pos) (value |= (1U << pos))
#define CLEAR_BIT(value, pos) (value &= (~(1U << pos)))

#define IS_MAC_BROADCAST_ADDR(mac)                                             \
        (mac[0] == 0xFF && mac[1] == 0xFF && mac[2] == 0xFF &&                 \
         mac[3] == 0xFF && mac[4] == 0xFF && mac[5] == 0xFF)

#endif