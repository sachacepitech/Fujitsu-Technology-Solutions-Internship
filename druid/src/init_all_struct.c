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

void init_usb_device_info_struct(usb_device_info_t *usb_device_info)
{
    usb_device_info->vendor_id = NULL;
    usb_device_info->vendor_name = NULL;
    usb_device_info->product_id = NULL;
    usb_device_info->product_name = NULL;
}

int init_struct_usb_db(usb_db_t *usb_db, size_t allocated_capacity)
{
    usb_db->entries = malloc(sizeof(usb_db_entry_t) * allocated_capacity);
    if (usb_db->entries == NULL)
        return MAJOR_ERROR;    
    usb_db->count = 0;
    return EXIT_SUCCESS;
}

void init_struct_temp_data(usb_db_entry_t *usb_db_entry)
{
    usb_db_entry->vendor_id = NULL;
    usb_db_entry->vendor_name = NULL;
    usb_db_entry->product_id = NULL;
    usb_db_entry->product_name = NULL;
}

void init_struct_unknown_usb_db_entry(usb_db_entry_t *unknown)
{
    unknown->vendor_id = NULL;
    unknown->vendor_name = strdup(UNKNOWN_DEVICE_MESSAGE);
    unknown->product_id = NULL;
    unknown->product_name = strdup(UNKNOWN_DEVICE_MESSAGE);
}
