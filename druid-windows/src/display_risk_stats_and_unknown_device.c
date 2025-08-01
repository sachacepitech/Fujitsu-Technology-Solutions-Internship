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
 * @brief Displays detailed information for a fully known USB device
 *
 * Prints vendor and product IDs, and both system-detected and database-matched names.
 * The device is considered low risk as both IDs fully match an entry in the database.
 *
 * @details void display_known_usb_device(
 *             usb_device_info_t *usb_device_info,
 *             usb_db_entry_t *usb_db_entry,
 *             usb_risk_stats_stats_t *usb_risk_stats,
 *             FILE *output_file)
 * @param usb_device_info Pointer to the structure containing current USB device info
 * @param usb_db_entry Pointer to the matching USB database entry (vendor and product matched)
 * @param usb_risk_stats Pointer to the risk statistics structure to update the low risk counter
 * @param output_file Optional file pointer to write output (if not NULL)
 */
void display_known_usb_device(usb_device_info_t *usb_device_info,
    usb_db_entry_t *usb_db_entry, usb_risk_stats_stats_t *usb_risk_stats,
    FILE *output_file)
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
    if (output_file != NULL) {
        fprintf(output_file, 
        "+-------------------------------------------- Known USB Device n°%lu --------------------------------------------/\n"
        "│ VendorID  (%s)   │   ProductID (%s)\n"
        "│\n"
        "│ From System:\n"
        "│     Vendor Name (%s)   │   Product Name (%s)\n"
        "│\n"
        "│ From Database:\n"
        "│     Vendor Name (%s)   │   Product Name (%s)\n"
        "│\n"
        "+------------------------------------------------------------------------------------------------------------/\n\n",
        usb_risk_stats->seen_count,
        usb_device_info->vendor_id,
        usb_device_info->product_id,
        usb_device_info->vendor_name,
        usb_device_info->product_name,
        usb_db_entry->vendor_name,
        usb_db_entry->product_name);
    }
}

/**
 * @brief Displays detailed information for a partially known USB device
 *
 * Prints the vendor ID (matched) and product ID (unknown), including vendor/product names
 * from both the system and partially matching entry in the database.
 * This device is categorized as medium risk.
 *
 * @details void display_partially_known_usb_device(
 *             usb_device_info_t *usb_device_info,
 *             usb_db_entry_t *usb_db_entry,
 *             usb_risk_stats_stats_t *usb_risk_stats,
 *             FILE *output_file)
 * @param usb_device_info Pointer to the structure containing current USB device info
 * @param usb_db_entry Pointer to the partially matching USB database entry (vendor matched only)
 * @param usb_risk_stats Pointer to the risk statistics structure to update the medium risk counter
 * @param output_file Optional file pointer to write output (if not NULL)
 */
void display_partially_known_usb_device(usb_device_info_t *usb_device_info,
    usb_db_entry_t *usb_db_entry, usb_risk_stats_stats_t *usb_risk_stats,
    FILE *output_file)
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
     if (output_file != NULL) {
        fprintf(output_file, 
        "+-------------------------------------------- Partially Known USB Device n°%lu ----------------------------------/\n"
        "│ VendorID  (%s)   │   ProductID (Unknown : %s)\n"
        "│\n"
        "│ From System:\n"
        "│     Vendor Name (%s)   │   Product Name (%s)\n"
        "│\n"
        "│ From Database:\n"
        "│     Vendor Name (%s)   │   Product Name (%s)\n"
        "│\n"
        "+------------------------------------------------------------------------------------------------------------/\n\n",
        usb_risk_stats->seen_count,
        usb_device_info->vendor_id,
        usb_device_info->product_id,
        usb_device_info->vendor_name,
        usb_device_info->product_name,
        usb_db_entry->vendor_name,
        usb_db_entry->product_name);
    }
}

/**
 * @brief Displays detailed information for an unknown USB device
 *
 * Prints vendor and product IDs as unknown, and lists the system-detected names
 * No match is found in the database, so the device is marked as major risk
 *
 * @details void display_unknown_usb_device(
 *             usb_device_info_t *usb_device_info,
 *             usb_db_entry_t *usb_db_entry,
 *             usb_risk_stats_stats_t *usb_risk_stats,
 *             FILE *output_file)
 * @param usb_device_info Pointer to the structure containing current USB device info
 * @param usb_db_entry Pointer to the database entry (likely empty/placeholder)
 * @param usb_risk_stats Pointer to the risk statistics structure to update the major risk counter
 * @param output_file Optional file pointer to write output (if not NULL)
 */
void display_unknown_usb_device(usb_device_info_t *usb_device_info,
    usb_db_entry_t *usb_db_entry, usb_risk_stats_stats_t *usb_risk_stats,
    FILE *output_file)
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
    if (output_file != NULL) {
        fprintf(output_file, 
        "+-------------------------------------------- Unknown USB Device n°%lu ------------------------------------------/\n"
        "│ VendorID  (Unknown : %s)   │   ProductID (Unknown : %s)\n"
        "│\n"
        "│ From System:\n"
        "│     Vendor Name (%s)   │   Product Name (%s)\n"
        "│\n"
        "│ From Database:\n"
        "│     Vendor Name (%s)   │   Product Name (%s)\n"
        "│\n"
        "+------------------------------------------------------------------------------------------------------------/\n\n",
        usb_risk_stats->seen_count,
        usb_device_info->vendor_id,
        usb_device_info->product_id,
        usb_device_info->vendor_name,
        usb_device_info->product_name,
        usb_db_entry->vendor_name,
        usb_db_entry->product_name);
    }
}

/**
 * @brief Displays a summary table of detected USB risk levels
 *
 * Prints the count of devices categorized as low, medium, and major risk
 * Output is printed to the console and optionally written to an output file
 *
 * @details void display_risk_table(
 *             usb_risk_stats_stats_t *usb_risk_stats,
 *             FILE *output_file)
 * @param usb_risk_stats Pointer to the structure containing aggregated risk counters
 * @param output_file Optional file pointer to write output (if not NULL)
 */
void display_risk_table(usb_risk_stats_stats_t *usb_risk_stats, FILE *output_file)
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
    if (output_file != NULL) {
        fprintf(output_file, 
        "+--------- Risk table ---------/\n"
        "│ Number Low Risk    :  %lu \n"
        "│\n"
        "│ Number Medium Risk :  %lu \n"
        "│\n"
        "│ Number Major Risk  :  %lu \n"
        "+-----------------------------/\n\n",
    usb_risk_stats->low, usb_risk_stats->medium, usb_risk_stats->major);
    }
}
