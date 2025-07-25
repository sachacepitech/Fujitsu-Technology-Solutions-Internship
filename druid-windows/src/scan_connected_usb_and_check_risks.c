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
#include <windows.h>
#include <setupapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "druid.h"
#include "seen_devices.h"

#include <ctype.h>

void str_to_lower(char *str) {
    for (; *str; ++str) {
        *str = (char)tolower((unsigned char)*str);
    }
}

/**
 * @brief Retrieves vendor and product information from a USB device
 *
 * Extracts vendor ID, vendor name, product ID, and product name
 * from the current device using system device properties.
 * 
 * @details static void get_vendor_product_device(
 *             usb_tools_t *usb_tools,
 *             usb_device_info_t *usb_device_info,
 *             SP_DEVINFO_DATA* dev_info_data)
 * 
 * @param usb_tools Pointer to the usb_tools_t structure containing the current device context
 * @param usb_device_info Pointer to the usb_device_info_t structure to store extracted data; 
 *        memory for strings is dynamically allocated and must be freed by the caller
 * @param dev_info_data Pointer to SP_DEVINFO_DATA for the current device
 * 
 * @return None (void)
 */
void get_vendor_product_device(usb_tools_t *usb_tools,
    usb_device_info_t *usb_device_info, SP_DEVINFO_DATA* dev_info_data)
{
    char hardware_id[512];
    char vendor_name[256];
    char product_name[256];
    char vendor_id[5];
    char product_id[5];

    get_device_property(usb_tools->device_info_set, dev_info_data, SPDRP_HARDWAREID, hardware_id, sizeof(hardware_id));
    extract_vendor_and_product_ids_from_hardware_id(hardware_id, vendor_id, product_id);
    str_to_lower(vendor_id);
    str_to_lower(product_id);
    get_device_property(usb_tools->device_info_set, dev_info_data, SPDRP_MFG, vendor_name, sizeof(vendor_name));
    get_device_property(usb_tools->device_info_set, dev_info_data, SPDRP_DEVICEDESC, product_name, sizeof(product_name));

    usb_device_info->vendor_id = strdup(vendor_id);
    usb_device_info->product_id = strdup(product_id);
    usb_device_info->vendor_name = strdup(vendor_name);
    usb_device_info->product_name = strdup(product_name);
}


/**
 * @brief Checks if the current USB device has already been processed
 *
 * Compares the current device's vendor and product IDs
 * against the list of already seen devices to avoid redundant processing.
 * 
 * @details static int check_already_seen(
 *             usb_device_info_t *usb_device_info,
 *             long unsigned int seen_count)
 * 
 * @param usb_device_info Pointer to the usb_device_info_t structure containing current device info
 * @param seen_count Number of devices already processed
 * 
 * @return Exit code:
 *         - 0      (EXIT_SUCCESS) if the device was already processed
 *         - -1     (UNSEEN) if the device was not yet seen
 */
static int check_already_seen(usb_device_info_t *usb_device_info, long unsigned int seen_count)
{
    for (long unsigned int k = 0; k < seen_count; ++k) {
        if (strcmp(usb_device_info->vendor_id, seen_devices[k].vendor_id) == 0 &&
            strcmp(usb_device_info->product_id, seen_devices[k].product_id) == 0) {
            return EXIT_SUCCESS;
        }
    }
    return UNSEEN;
}


/**
 * @brief Adds the current USB device to the list of seen devices
 *
 * Stores the vendor and product IDs of the current USB device
 * into a global list if the maximum device limit has not been reached.
 * 
 * @details static void add_to_seen(
 *             usb_device_info_t *usb_device_info,
 *             long unsigned int *seen_count)
 * 
 * @param usb_device_info Pointer to the usb_device_info_t structure containing current device info
 * @param seen_count Pointer to the current count of seen devices (incremented if added)
 * 
 * @return None (void)
 */
static void add_to_seen(usb_device_info_t *usb_device_info, long unsigned int *seen_count)
{
    if (*seen_count < MAX_SEEN_DEVICES) {
        seen_devices[*seen_count].vendor_id = strdup(usb_device_info->vendor_id);
        seen_devices[*seen_count].product_id = strdup(usb_device_info->product_id);
        ++(*seen_count);
    }
}


/**
 * @brief Checks if a connected USB device exists in the known database
 *
 * Compares the vendor and product IDs of the current USB device
 * against the loaded USB database and updates the risk statistics
 * accordingly based on match level (full, partial, or unknown).
 * 
 * @details static void check_usb_exist(
 *             usb_db_t *usb_db,
 *             usb_db_entry_t *usb_db_entry,
 *             usb_device_info_t *usb_device_info,
 *             usb_risk_stats_stats_t *usb_risk_stats)
 * 
 * @param usb_db Pointer to the usb_db_t structure containing loaded database entries
 * @param usb_db_entry Pointer to a reusable usb_db_entry_t structure (for matches)
 * @param usb_device_info Pointer to the usb_device_info_t structure containing current device info
 * @param usb_risk_stats Pointer to the usb_risk_stats_stats_t structure to update statistics
 * 
 * @return None (void)
 */
static void check_usb_exist(usb_db_t *usb_db, usb_db_entry_t *usb_db_entry,
    usb_device_info_t *usb_device_info, usb_risk_stats_stats_t *usb_risk_stats)
{
    bool match_vendor_and_product = false;
    bool match_vendor_only = false;
    usb_db_entry_t *matching_entry = NULL;
    usb_db_entry_t unknown = {0};

    for (long unsigned int i = 0; i < usb_db->count; ++i) {
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
 * @brief Frees allocated memory associated with USB device information and details
 *
 * Releases memory allocated for vendor and product IDs and names,
 * as well as the device interface detail structure.
 * 
 * @param usb_device_info Pointer to the usb_device_info_t structure containing device info to free
 * @param detail Pointer to PSP_DEVICE_INTERFACE_DETAIL_DATA_A structure to free
 * 
 * @return None (void)
 */
void free_usb_device_and_details(usb_device_info_t *usb_device_info,
    PSP_DEVICE_INTERFACE_DETAIL_DATA_A detail)
{
    free(usb_device_info->vendor_id);
    free(usb_device_info->product_id);
    free(usb_device_info->vendor_name);
    free(usb_device_info->product_name);
    free(detail);
}


/**
 * @brief Scans connected USB devices and checks for potential risks
 *
 * Loads the USB database from file, iterates through connected USB devices,
 * compares each device against the database, and updates risk statistics.
 * 
 * @details int scan_connected_usb_and_check_risks(
 *             usb_tools_t *usb_tools,
 *             usb_device_info_t *usb_device_info,
 *             usb_db_entry_t *usb_db_entry,
 *             cli_args_t *cli_args)
 * 
 * @param usb_tools Pointer to the usb_tools_t structure used for device enumeration
 * @param usb_device_info Pointer to the usb_device_info_t structure for storing device info
 * @param usb_db_entry Pointer to the usb_db_entry_t structure for matching database entries
 * @param cli_args Pointer to the cli_args_t structure containing CLI arguments
 * 
 * @return Exit code:
 *         - 0      (EXIT_SUCCESS) when scanning and risk checking complete
 *         - 84     (EXIT_ERROR) if database loading fails
 */
int scan_connected_usb_and_check_risks(usb_tools_t *usb_tools, usb_device_info_t *usb_device_info,
    usb_db_entry_t *usb_db_entry, cli_args_t *cli_args)
{
    usb_db_t usb_db = {0};
    usb_risk_stats_stats_t usb_risk_stats = {0};
    SP_DEVICE_INTERFACE_DATA interface_data = {0};
    SP_DEVINFO_DATA dev_info_data = {0};
    PSP_DEVICE_INTERFACE_DETAIL_DATA_A detail = NULL;

    if (load_usb_db_from_file(&usb_db, usb_db_entry, cli_args) == EXIT_ERROR) {
        free_usb_db(&usb_db);
        return EXIT_ERROR;
    }
    interface_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    dev_info_data.cbSize = sizeof(SP_DEVINFO_DATA);
    usb_tools->index = 0;
    while (SetupDiEnumDeviceInterfaces(
            usb_tools->device_info_set, NULL, &USB_IF_GUID, 
            usb_tools->index++, &interface_data)) {
        detail = get_device_interface_detail(usb_tools->device_info_set,
            &interface_data, &dev_info_data);
        if (detail == NULL)
            continue;
        get_vendor_product_device(usb_tools, usb_device_info, &dev_info_data);
        if (check_already_seen(usb_device_info, usb_risk_stats.seen_count) == EXIT_SUCCESS) {
            free_usb_device_and_details(usb_device_info, detail);
            continue;
        }
        check_usb_exist(&usb_db, usb_db_entry, usb_device_info, &usb_risk_stats);
        free_usb_device_and_details(usb_device_info, detail);
    }
    free_usb_db(&usb_db);
    display_risk_table(&usb_risk_stats);
    return EXIT_SUCCESS;
}
