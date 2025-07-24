/**
 * @name druid (Detection Rogue USB and Illegitimate Devices)
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file scan_connected_usb_and_check_risks.c
 * @brief Scans connected usb devices and evaluates potential security risks
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
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <systemd/sd-device.h>
#include "druid.h"
#include "seen_devices.h"

/**
 * @brief Retrieves vendor and product information from a USB device
 *
 * extracts vendor ID, vendor name, product ID, and product name
 * from the current device using systemd device properties
 * 
 * @details static void get_vendor_product_device(
 *             usb_tools_t *usb_tools,
 *             usb_device_info_t *usb_device_info)
 * @param usb_tools Pointer to the usb_tools_t structure containing the current device
 * @param usb_device_info Pointer to the usb_device_info_t structure to store extracted data
 * @return None (void)
 */
static void get_vendor_product_device(usb_tools_t *usb_tools,
    usb_device_info_t *usb_device_info)
{
    sd_device_get_property_value(usb_tools->device, VENDOR_ID,
        &usb_device_info->vendor_id);
    sd_device_get_property_value(usb_tools->device, VENDOR_NAME,
        &usb_device_info->vendor_name);
    sd_device_get_property_value(usb_tools->device, PRODUCT_ID,
        &usb_device_info->product_id);
    sd_device_get_property_value(usb_tools->device, PRODUCT_NAME,
        &usb_device_info->product_name);
}

/**
 * @brief Checks if the current USB device has already been processed
 *
 * compares the current device's vendor and product IDs
 * against the list of already seen devices to avoid redundant processing
 * 
 * @details static int check_already_seen(
 *             usb_tools_t *usb_tools,
 *             usb_device_info_t *usb_device_info,
 *             size_t seen_count,
 *             bool already_seen)
 * @param usb_tools Pointer to the usb_tools_t structure used for device enumeration
 * @param usb_device_info Pointer to the usb_device_info_t structure containing current device info
 * @param seen_count Number of devices already processed
 * @param already_seen Boolean flag to indicate if the device was already seen
 * @return Exit code:
 *         - 0      (EXIT_SUCCESS) if the device was already processed
 *         - -1     (UNSEEN) if the device was not yet seen
 */
static int check_already_seen(usb_tools_t *usb_tools,
    usb_device_info_t *usb_device_info, size_t seen_count, bool already_seen)
{
    for (size_t k = 0; k < seen_count; ++k) {
        if (strcmp(usb_device_info->vendor_id, seen_devices[k].vendor_id) == SUCCESS &&
            strcmp(usb_device_info->product_id, seen_devices[k].product_id) == SUCCESS) {
            already_seen = true;
            break;
        }
    }
    if (already_seen == true) {
        usb_tools->device = sd_device_enumerator_get_device_next(
            usb_tools->enumerator);
        return EXIT_SUCCESS;
    }
    return UNSEEN;
}

/**
 * @brief Adds the current USB device to the list of seen devices
 *
 * stores the vendor and product IDs of the current USB device
 * into a global list if the maximum device limit has not been reached
 * 
 * @details static void add_to_seen(
 *             usb_device_info_t *usb_device_info,
 *             size_t *seen_count)
 * @param usb_device_info Pointer to the usb_device_info_t structure containing current device info
 * @param seen_count Pointer to the current count of seen devices (incremented if added)
 * @return None (void)
 */
static void add_to_seen(usb_device_info_t *usb_device_info, size_t *seen_count)
{
    if (*seen_count < MAX_SEEN_DEVICES) {
        seen_devices[*seen_count].vendor_id = usb_device_info->vendor_id
            ? usb_device_info->vendor_id : strdup(usb_device_info->vendor_id);
        seen_devices[*seen_count].product_id = usb_device_info->product_id
            ? usb_device_info->product_id : strdup(usb_device_info->product_id);
        ++(*seen_count);
    }
}

/**
 * @brief Checks if a connected USB device exists in the known database
 *
 * compares the vendor and product IDs of the current USB device
 * against the loaded USB database and updates the risk statistics
 * accordingly based on match level (full, partial, or unknown)
 * 
 * @details static void check_usb_exist(
 *             usb_db_t *usb_db,
 *             usb_db_entry_t *usb_db_entry,
 *             usb_device_info_t *usb_device_info,
 *             usb_risk_stats_stats_t *usb_risk_stats)
 * @param usb_db Pointer to the usb_db_t structure containing loaded database entries
 * @param usb_db_entry Pointer to a reusable usb_db_entry_t structure (for matches)
 * @param usb_device_info Pointer to the usb_device_info_t structure containing current device info
 * @param usb_risk_stats Pointer to the usb_risk_stats_stats_t structure to update statistics
 * @return None (void)
 */
static void check_usb_exist(usb_db_t *usb_db, usb_db_entry_t *usb_db_entry,
    usb_device_info_t *usb_device_info, usb_risk_stats_stats_t *usb_risk_stats)
{
    bool match_vendor_and_product = false;
    bool match_vendor_only = false;
    usb_db_entry_t *matching_entry = NULL;
    usb_db_entry_t unknown = {0};

    for (size_t i = 0; i < usb_db->count; ++i) {
        usb_db_entry = &usb_db->entries[i];
        if (strcmp(usb_device_info->vendor_id, usb_db_entry->vendor_id) == SUCCESS) {
            if (strcmp(usb_device_info->product_id, usb_db_entry->product_id) == SUCCESS) {
                match_vendor_and_product = true;
                matching_entry = usb_db_entry;
                break;
            } else if (matching_entry == NULL) {
                match_vendor_only = true;
                matching_entry = usb_db_entry;
            }
        }
    }
    if (match_vendor_and_product == true) {
        display_known_usb_device(usb_device_info, matching_entry, usb_risk_stats);
    } else if (match_vendor_only == true) {
        display_partially_known_usb_device(usb_device_info, matching_entry, usb_risk_stats);
    } else {
        init_struct_unknown_usb_db_entry(&unknown);
        display_unknown_usb_device(usb_device_info, &unknown, usb_risk_stats);
        free_unknown_usb_db_entry(&unknown);
    }
    add_to_seen(usb_device_info, &usb_risk_stats->seen_count);
}

/**
 * @brief Scans connected USB devices and checks for potential risks
 *
 * loads the USB database from file, iterates through connected USB devices,
 * compares each device against the database, and updates risk statistics
 * 
 * @details int scan_connected_usb_and_check_risks(
 *             usb_tools_t *usb_tools,
 *             usb_device_info_t *usb_device_info,
 *             usb_db_entry_t *usb_db_entry,
 *             cli_args_t *cli_args)
 * @param usb_tools Pointer to the usb_tools_t structure used for device enumeration
 * @param usb_device_info Pointer to the usb_device_info_t structure for storing device info
 * @param usb_db_entry Pointer to the usb_db_entry_t structure for matching database entries
 * @param cli_args Pointer to the cli_args_t structure containing CLI arguments
 * @return Exit code:
 *         - 0      (EXIT_SUCCESS) when scanning and risk checking complete
 *         - 84     (EXIT_ERROR) if database loading fails
 */
int scan_connected_usb_and_check_risks(usb_tools_t *usb_tools, usb_device_info_t *usb_device_info,
    usb_db_entry_t *usb_db_entry, cli_args_t *cli_args)
{
    usb_db_t usb_db = {0};
    usb_risk_stats_stats_t usb_risk_stats = {0};
    bool already_seen = false;

    if (load_usb_db_from_file(&usb_db, usb_db_entry, cli_args) == EXIT_ERROR) {
        free_usb_db(&usb_db);
        return EXIT_ERROR;
    }
    usb_tools->device = sd_device_enumerator_get_device_first(
        usb_tools->enumerator);
    while (usb_tools->device != NULL) {
        already_seen = false;
        get_vendor_product_device(usb_tools, usb_device_info);
        if (check_already_seen(usb_tools, usb_device_info, usb_risk_stats.seen_count, already_seen) == SUCCESS)
            continue;
        check_usb_exist(&usb_db, usb_db_entry, usb_device_info, &usb_risk_stats);
        usb_tools->device = sd_device_enumerator_get_device_next(
            usb_tools->enumerator);
    }
    free_usb_db(&usb_db);
    display_risk_table(&usb_risk_stats);
    return EXIT_SUCCESS;
}
