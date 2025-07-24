/**
 * @name druid
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file init_struct_db_and_device.c 
 * @brief Initializes database and device struct
 * @date 17 July 2025
 * @copyright Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0 DEED)
 * 
 * This file is part of the "druid" repository.
 * 
 * You can use, modify, and distribute this code under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0 DEED).
 * See the full license at: https://creativecommons.org/licenses/by-sa/4.0/deed.fr
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <systemd/sd-device.h>
#include "druid.h"

/**
 * @brief Initializes the usb_tools_t structure to default values
 *
 * sets the internal pointers of the usb_tools_t structure
 * to NULL in preparation for USB enumeration setup
 * 
 * @details void init_struct_usb_tools(usb_tools_t *usb_tools)
 * @param usb_tools Pointer to the usb_tools_t structure to be initialized
 * @return None (void)
 */
void init_struct_usb_tools(usb_tools_t *usb_tools)
{
    usb_tools->device = NULL;
    usb_tools->enumerator = NULL;
}

/**
 * @brief Initializes the usb_device_info_t structure to default values
 *
 * sets all fields of the usb_device_info_t structure
 * to NULL in preparation for device data extraction
 * 
 * @details void init_struct_usb_device_info(usb_device_info_t *usb_device_info)
 * @param usb_device_info Pointer to the usb_device_info_t structure to be initialized
 * @return None (void)
 */
void init_struct_usb_device_info(usb_device_info_t *usb_device_info)
{
    usb_device_info->vendor_id = NULL;
    usb_device_info->vendor_name = NULL;
    usb_device_info->product_id = NULL;
    usb_device_info->product_name = NULL;
}

/**
 * @brief Initializes the usb_db_t structure with allocated capacity
 *
 * allocates memory for storing USB database entries and
 * initializes the entry count to zero
 * 
 * @details int init_struct_usb_db(usb_db_t *usb_db, size_t allocated_capacity)
 * @param usb_db Pointer to the usb_db_t structure to initialize
 * @param allocated_capacity Number of usb_db_entry_t elements to allocate
 * @return Exit code:
 *         - 0      (EXIT_SUCCESS) on successful memory allocation
 *         - 84     (EXIT_ERROR) if memory allocation fails
 */
int init_struct_usb_db(usb_db_t *usb_db, size_t allocated_capacity)
{
    usb_db->entries = malloc(sizeof(usb_db_entry_t) * allocated_capacity);
    if (usb_db->entries == NULL)
        return EXIT_ERROR;    
    usb_db->count = 0;
    return EXIT_SUCCESS;
}

/**
 * @brief Initializes a temporary usb_db_entry_t structure
 *
 * sets all fields of the usb_db_entry_t structure
 * (vendor and product identifiers and names) to NULL
 * before parsing or data assignment
 * 
 * @details void init_struct_usb_db_entry(usb_db_entry_t *usb_db_entry)
 * @param usb_db_entry Pointer to the usb_db_entry_t structure to be initialized
 * @return None (void)
 */
void init_struct_usb_db_entry(usb_db_entry_t *usb_db_entry)
{
    usb_db_entry->vendor_id = NULL;
    usb_db_entry->vendor_name = NULL;
    usb_db_entry->product_id = NULL;
    usb_db_entry->product_name = NULL;
}

/**
 * @brief Initializes a usb_db_entry_t structure for unknown USB devices
 *
 * sets vendor and product identifiers to NULL, and assigns default
 * placeholder strings for vendor and product names to indicate an unknown device
 * 
 * @details void init_struct_unknown_usb_db_entry(usb_db_entry_t *unknown)
 * @param unknown Pointer to the usb_db_entry_t structure to be initialized as unknown
 * @return None (void)
 */
void init_struct_unknown_usb_db_entry(usb_db_entry_t *unknown)
{
    unknown->vendor_id = NULL;
    unknown->vendor_name = strdup(UNKNOWN_DEVICE_MESSAGE);
    unknown->product_id = NULL;
    unknown->product_name = strdup(UNKNOWN_DEVICE_MESSAGE);
}
