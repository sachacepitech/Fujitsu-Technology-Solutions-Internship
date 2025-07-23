/**
 * @name druid
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file comparator.c
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
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <systemd/sd-device.h>
#include "druid.h"
#include "already_seen.h"

static void get_vendor_product_device(usb_tools_t *usb_tools,
    con_data_usb_t *con_data_usb)
{
    sd_device_get_property_value(usb_tools->device, VENDOR_ID,
        &con_data_usb->vendor_id);
    sd_device_get_property_value(usb_tools->device, VENDOR_NAME,
        &con_data_usb->vendor_name);
    sd_device_get_property_value(usb_tools->device, PRODUCT_ID,
        &con_data_usb->product_id);
    sd_device_get_property_value(usb_tools->device, PRODUCT_NAME,
        &con_data_usb->product_name);
}

static int check_already_seen(usb_tools_t *usb_tools,
    con_data_usb_t *con_data_usb, size_t seen_count, bool already_seen)
{
    for (size_t k = 0; k < seen_count; ++k) {
        if (strcmp(con_data_usb->vendor_id, seen[k].vendor_id) == SUCCESS &&
            strcmp(con_data_usb->product_id, seen[k].product_id) == SUCCESS) {
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

static void add_to_seen(con_data_usb_t *con_data_usb, size_t *seen_count)
{
    if (*seen_count < MAX_SEEN_DEVICES) {
        seen[*seen_count].vendor_id = con_data_usb->vendor_id
            ? con_data_usb->vendor_id : strdup(con_data_usb->vendor_id);
        seen[*seen_count].product_id = con_data_usb->product_id
            ? con_data_usb->product_id : strdup(con_data_usb->product_id);
        ++(*seen_count);
    }
}

static void check_usb_exist(usb_db_t *usb_db, temp_data_usb_t *temp_data_usb,
    con_data_usb_t *con_data_usb, usb_risk_t *usb_risk)
{
    bool match_vendor_and_product = false;
    bool match_vendor_only = false;
    temp_data_usb_t *matching_entry = NULL;
    temp_data_usb_t unknown = {0};

    for (size_t i = 0; i < usb_db->count; ++i) {
        temp_data_usb = &usb_db->entries[i];
        if (strcmp(con_data_usb->vendor_id, temp_data_usb->vendor_id) == SUCCESS) {
            if (strcmp(con_data_usb->product_id, temp_data_usb->product_id) == SUCCESS) {
                match_vendor_and_product = true;
                matching_entry = temp_data_usb;
                break;
            } else if (matching_entry == NULL) {
                match_vendor_only = true;
                matching_entry = temp_data_usb;
            }
        }
    }
    if (match_vendor_and_product == true) {
        display_couple_vendor_product_device(con_data_usb, matching_entry, usb_risk);
    } else if (match_vendor_only == true) {
        display_vendor_found_product_unknown_device(con_data_usb, matching_entry, usb_risk);
    } else {
        init_struct_unknown_temp_data_usb(&unknown);
        display_vendor_unknown_product_unknown_device(con_data_usb, &unknown, usb_risk);
        free_unknown_temp_data_usb(&unknown);
    }
    add_to_seen(con_data_usb, &usb_risk->seen_count);
}

int comparator(usb_tools_t *usb_tools, con_data_usb_t *con_data_usb,
    temp_data_usb_t *temp_data_usb, param_t *param)
{
    usb_db_t usb_db = {0};
    usb_risk_t usb_risk = {0};
    bool already_seen = false;

    if (fill_struct_db_temp(&usb_db, temp_data_usb, param) == MAJOR_ERROR) {
        free_usb_db(&usb_db);
        return MAJOR_ERROR;
    }
    usb_tools->device = sd_device_enumerator_get_device_first(
        usb_tools->enumerator);
    while (usb_tools->device != NULL) {
        already_seen = false;
        get_vendor_product_device(usb_tools, con_data_usb);
        if (check_already_seen(usb_tools, con_data_usb, usb_risk.seen_count, already_seen) == SUCCESS)
            continue;
        check_usb_exist(&usb_db, temp_data_usb, con_data_usb, &usb_risk);
        usb_tools->device = sd_device_enumerator_get_device_next(
            usb_tools->enumerator);
    }
    free_usb_db(&usb_db);
    display_risk_table(&usb_risk);
    return EXIT_SUCCESS;
}
