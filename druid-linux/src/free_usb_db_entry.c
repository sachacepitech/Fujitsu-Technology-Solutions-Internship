/**
 * @name druid (Detection Rogue USB and Illegitimate Devices)
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file free_usb_db_entry.c
 * @brief free usb database entry
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
 * @brief Frees memory allocated for an unknown usb_db_entry_t structure
 *
 * deallocates the vendor and product name strings
 * that were previously set for an unknown USB device
 * 
 * @details void free_unknown_usb_db_entry(usb_db_entry_t *unknown)
 * @param unknown Pointer to the usb_db_entry_t structure to be freed
 * @return None (void)
 */
void free_unknown_usb_db_entry(usb_db_entry_t *unknown)
{
    free(unknown->vendor_name);
    free(unknown->product_name);
}

/**
 * @brief Frees all memory allocated within a usb_db_t structure
 *
 * iterates over each usb_db_entry_t in the database to free
 * vendor and product identifiers and names, then releases the entries array
 * 
 * @details void free_usb_db(usb_db_t *usb_db)
 * @param usb_db Pointer to the usb_db_t structure to be freed
 * @return None (void)
 */
void free_usb_db(usb_db_t *usb_db)
{
    for (size_t i = 0; i < usb_db->count; i++) {
        free(usb_db->entries[i].vendor_id);
        free(usb_db->entries[i].vendor_name);
        free(usb_db->entries[i].product_id);
        free(usb_db->entries[i].product_name);
    }
    free(usb_db->entries);
}
