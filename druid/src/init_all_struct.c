/**
 * @name druid
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file init_all_struct.c 
 * @date 17 July 2025
 * @copyright Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0)
 * 
 * This file is part of the "druid" repository.
 * 
 * You can use, modify, and distribute this code under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0).
 * See the full license at: https://creativecommons.org/licenses/by-sa/4.0/deed.fr
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <systemd/sd-device.h>
#include "druid.h"

void init_usb_tools_struct(usb_tools_t *usb_tools)
{
    usb_tools->device = NULL;
    usb_tools->enumerator = NULL;
}

void init_con_data_usb_struct(con_data_usb_t *con_data_usb)
{
    con_data_usb->vendor_id = NULL;
    con_data_usb->vendor_name = NULL;
    con_data_usb->product_id = NULL;
    con_data_usb->product_name = NULL;
}

int init_struct_usb_db(usb_db_t *usb_db, size_t mem_allocated)
{
    usb_db->entries = malloc(sizeof(temp_data_usb_t) * mem_allocated);
    if (usb_db->entries == NULL)
        return MAJOR_ERROR;    
    usb_db->count = 0;
    return EXIT_SUCCESS;
}

void init_struct_temp_data(temp_data_usb_t *temp_data_usb)
{
    temp_data_usb->vendor_id = NULL;
    temp_data_usb->vendor_name = NULL;
    temp_data_usb->product_id = NULL;
    temp_data_usb->product_name = NULL;
}

void init_struct_unknown_temp_data_usb(temp_data_usb_t *unknown)
{
    unknown->vendor_id = NULL;
    unknown->vendor_name = strdup(UNKNOWN_DEVICE_MESSAGE);
    unknown->product_id = NULL;
    unknown->product_name = strdup(UNKNOWN_DEVICE_MESSAGE);
}
