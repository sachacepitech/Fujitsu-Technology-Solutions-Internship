/**
 * @name druid (Detection Rogue USB and Illegitimate Devices)
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file display_risk_stats_and_unknown_device.c
 * @brief display risk stats and unknown devices
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
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "druid.h"

/**
 * @brief Displays detailed info of a fully known USB device
 *
 * Prints vendor and product IDs, names from both system and database,
 * highlighting that the device is fully recognized (couple vendor and product IDs) and counts it as low risk.
 * 
 * @details void display_known_usb_device(
 *             usb_device_info_t *usb_device_info,
 *             usb_db_entry_t *usb_db_entry,
 *             usb_risk_stats_stats_t *usb_risk_stats)
 * @param usb_device_info Pointer to the structure containing current USB device info
 * @param usb_db_entry Pointer to the matching USB database entry
 * @param usb_risk_stats Pointer to the risk statistics structure to update counters
 * @return None (void)
 */
void display_known_usb_device(usb_device_info_t *usb_device_info,
    usb_db_entry_t *usb_db_entry, usb_risk_stats_stats_t *usb_risk_stats)
{
    printf(
        "+------------------------------------------------- Device number \033[1;32m%lu\033[0m -------------------------------------------------/\033[0m\n"
        "| VendorID  (\033[1;32m%s\033[0m)   |   ProductID (\033[1;32m%s\033[0m)\n"
        "|\n"
        "| \033[1;36mFrom System\033[0m:\n"
        "|     Vendor Name (\033[1;34m%s\033[0m)   |   Product Name (\033[1;34m%s\033[0m)\n"
        "|\n"
        "| \033[1;36mFrom Database\033[0m:\n"
        "|     Vendor Name (\033[1;34m%s\033[0m)   |   Product Name (\033[1;34m%s\033[0m)\n"
        "|\n"
        "+------------------------------------------------------------------------------------------------------------/\033[0m\n\n",
        usb_risk_stats->seen_count,
        usb_device_info->vendor_id,
        usb_device_info->product_id,
        usb_device_info->vendor_name,
        usb_device_info->product_name,
        usb_db_entry->vendor_name,
        usb_db_entry->product_name);
    ++usb_risk_stats->low;
}

/**
 * @brief Displays detailed info of a partially known USB device
 *
 * Prints vendor ID and notes the product ID as unknown, shows names from the system,
 * and partially matches vendor info from the database, marking the device as medium risk.
 * 
 * @details void display_partially_known_usb_device(
 *             usb_device_info_t *usb_device_info,
 *             usb_db_entry_t *usb_db_entry,
 *             usb_risk_stats_stats_t *usb_risk_stats)
 * @param usb_device_info Pointer to the structure containing current USB device info
 * @param usb_db_entry Pointer to the partially matching USB database entry (vendor matched only)
 * @param usb_risk_stats Pointer to the risk statistics structure to update counters
 * @return None (void)
 */
void display_partially_known_usb_device(usb_device_info_t *usb_device_info,
    usb_db_entry_t *usb_db_entry, usb_risk_stats_stats_t *usb_risk_stats)
{
    
    printf(
        "+------------------------------------------------- Device number \033[1;33m%lu\033[0m -------------------------------------------------/\033[0m\n"
        "| VendorID  (\033[1;32m%s\033[0m)   |   ProductID (\033[1;31mUnknown : %s\033[0m)\n"
        "|\n"
        "| \033[1;36mFrom System\033[0m:\n"
        "|     Vendor Name (\033[1;34m%s\033[0m)   |   Product Name (\033[1;34m%s\033[0m)\n"
        "|\n"
        "| \033[1;36mFrom Database\033[0m:\n"
        "|     Vendor Name (\033[1;31m%s\033[0m)   |   Product Name (\033[1;31m%s\033[0m)\n"
        "|\n"
        "+------------------------------------------------------------------------------------------------------------/\033[0m\n\n",
        usb_risk_stats->seen_count,
        usb_device_info->vendor_id,
        usb_device_info->product_id,
        usb_device_info->vendor_name,
        usb_device_info->product_name,
        usb_db_entry->vendor_name,
        usb_db_entry->product_name);
    ++usb_risk_stats->medium;
}

/**
 * @brief Displays detailed info of an unknown USB device
 *
 * Prints vendor ID and product ID as unknown, along with names from the system and database,
 * highlighting the device as a major risk and updating the risk statistics accordingly.
 * 
 * @details void display_unknown_usb_device(
 *             usb_device_info_t *usb_device_info,
 *             usb_db_entry_t *usb_db_entry,
 *             usb_risk_stats_stats_t *usb_risk_stats)
 * @param usb_device_info Pointer to the structure containing current USB device info
 * @param usb_db_entry Pointer to the USB database entry (typically empty or unknown)
 * @param usb_risk_stats Pointer to the risk statistics structure to update counters
 * @return None (void)
 */
void display_unknown_usb_device(usb_device_info_t *usb_device_info,
    usb_db_entry_t *usb_db_entry, usb_risk_stats_stats_t *usb_risk_stats)
{
    
    printf(
        "+------------------------------------------------- Device number \033[1;31m%lu\033[0m -------------------------------------------------/\033[0m\n"
        "| VendorID  (\033[1;31mUnknown : %s\033[0m)   |   ProductID (\033[1;31mUnknown : %s\033[0m)\n"
        "|\n"
        "| \033[1;36mFrom System\033[0m:\n"
        "|     Vendor Name (\033[1;31m%s\033[0m)   |   Product Name (\033[1;31m%s\033[0m)\n"
        "|\n"
        "| \033[1;36mFrom Database\033[0m:\n"
        "|     Vendor Name (\033[1;31m%s\033[0m)   |   Product Name (\033[1;31m%s\033[0m)\n"
        "|\n"
        "+------------------------------------------------------------------------------------------------------------/\033[0m\n\n",
        usb_risk_stats->seen_count,
        usb_device_info->vendor_id,
        usb_device_info->product_id,
        usb_device_info->vendor_name,
        usb_device_info->product_name,
        usb_db_entry->vendor_name,
        usb_db_entry->product_name);
    ++usb_risk_stats->major;
}

/**
 * @brief Displays a summary risk table for USB devices
 *
 * Prints the count of USB devices categorized as low, medium, and major risk,
 * using colored formatting for clear visualization.
 * 
 * @details void display_risk_table(usb_risk_stats_stats_t *usb_risk_stats)
 * @param usb_risk_stats Pointer to the structure containing risk counters
 * @return void
 */
void display_risk_table(usb_risk_stats_stats_t *usb_risk_stats)
{
     printf(
        "+--------- Risk table ---------/\033[0m\n"
        "| Number Low Risk    :  \033[1;32m%lu\033[0m \n"
        "|\n"
        "| Number Medium Risk :  \033[1;33m%lu\033[0m \n"
        "|\n"
        "| Number Major Risk  :  \033[1;31m%lu\033[0m \n"
        "+-----------------------------/\033[0m\n\n",
    usb_risk_stats->low, usb_risk_stats->medium, usb_risk_stats->major);

}
