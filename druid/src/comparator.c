/**
 * @name DRUID
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file comparator.c
 * @date 17 July 2025
 * @copyright Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0)
 * 
 * This file is part of the "DRUID" repository.
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

static int init_struct_usb_db(usb_db_t *usb_db, size_t mem_allocated)
{
    usb_db->entries = malloc(sizeof(temp_data_usb_t) * mem_allocated);
    if (usb_db->entries == NULL)
        return MAJOR_ERROR;    
    usb_db->count = 0;
    return EXIT_SUCCESS;
}

static void init_struct_temp_data(temp_data_usb_t *temp_data_usb)
{
    temp_data_usb->vendor_id = NULL;
    temp_data_usb->vendor_name = NULL;
    temp_data_usb->product_id = NULL;
    temp_data_usb->product_name = NULL;
}

static void remove_newline(char *str)
{
    size_t len = strlen(str);

    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

static void fill_struct_temp_data(temp_data_usb_t *temp_data_usb, char *line)
{
    char *vendor_id = strtok(line, FILE_SEPARATOR);
    char *vendor_name = strtok(NULL, FILE_SEPARATOR);
    char *product_id = strtok(NULL, FILE_SEPARATOR);
    char *product_name = strtok(NULL, FILE_SEPARATOR);

    remove_newline(product_name);
    temp_data_usb->vendor_id = vendor_id ? strdup(vendor_id) : NULL;
    temp_data_usb->vendor_name = vendor_name ? strdup(vendor_name) : NULL;
    temp_data_usb->product_id = product_id ? strdup(product_id) : NULL;
    temp_data_usb->product_name = product_name ? strdup(product_name) : NULL;
}

static int add_new_data(param_t *param, usb_db_t *usb_db,
    temp_data_usb_t *temp_data_usb, size_t mem_allocated)
{
    char *line = NULL;
    size_t n = 0;
    FILE *update_data_file = fopen(strcat(param->av[2], FILE_TYPE_PLUS_SEPARATOR), READ_MODE);

    if (update_data_file == NULL)
        return MAJOR_ERROR;
    while (getline(&line, &n, update_data_file) != EOF) {
        if (usb_db->count >= mem_allocated) {
            mem_allocated *= INCREASED_SIZE;
            usb_db->entries = realloc(usb_db->entries, sizeof(temp_data_usb_t)
                * mem_allocated);
            if (usb_db->entries == NULL)
                return MAJOR_ERROR;
        }
        temp_data_usb = &usb_db->entries[usb_db->count];
        init_struct_temp_data(temp_data_usb);
        fill_struct_temp_data(temp_data_usb, line);
        ++usb_db->count;
    }
    fclose(update_data_file);
    free(line);
    return EXIT_SUCCESS;
}

static int check_update_file(param_t *param, usb_db_t *usb_db,
    temp_data_usb_t *temp_data_usb, size_t mem_allocated)
{
    if (param->ac == 3 &&
        (strcmp(param->av[1], UPDATE_FLAG) == SUCCESS
        || strcmp(param->av[1], UPDATE_FLAG_OPTION) == SUCCESS)) {
            strtok(param->av[2], FILE_TYPE_SEPARATOR);
            if (strcmp(strtok(NULL, FILE_TYPE_SEPARATOR), FILE_TYPE) != SUCCESS) {
                printf("Error: Make sure it is a csv file.\n");
                return MAJOR_ERROR;
            } else {
                if (add_new_data(param, usb_db,
                    temp_data_usb, mem_allocated) == MAJOR_ERROR)
                    return MAJOR_ERROR;
            }
    } else
        return EXIT_SUCCESS;
    return EXIT_SUCCESS;
}

static int fill_struct_db_temp(usb_db_t *usb_db,
    temp_data_usb_t *temp_data_usb, param_t *param)
{
    FILE *data_file = fopen(DATA_FILE_PATH, READ_MODE);
    char *line = NULL;
    size_t n = 0;
    size_t mem_allocated = DEFAULT_SIZE;

    if (data_file == NULL)
        return MAJOR_ERROR;
    if (init_struct_usb_db(usb_db, mem_allocated) == MAJOR_ERROR)
        return MAJOR_ERROR;
    if (check_update_file(param, usb_db, temp_data_usb, mem_allocated) == MAJOR_ERROR)
        return MAJOR_ERROR;
    while (getline(&line, &n, data_file) != EOF) {
        if (usb_db->count >= mem_allocated) {
            mem_allocated *= INCREASED_SIZE;
            usb_db->entries = realloc(usb_db->entries, sizeof(temp_data_usb_t)
                * mem_allocated);
            if (usb_db->entries == NULL)
                return MAJOR_ERROR;
        }
        temp_data_usb = &usb_db->entries[usb_db->count];
        init_struct_temp_data(temp_data_usb);
        fill_struct_temp_data(temp_data_usb, line);
        ++usb_db->count;
    }
    free(line);
    fclose(data_file);
    return EXIT_SUCCESS;
}

static void get_vendor_product_device(usb_tools_t *usb_tools,
    con_data_usb_t *con_data_usb)
{
    sd_device_get_property_value(usb_tools->device, "ID_VENDOR_ID",
        &con_data_usb->vendor_id);
    sd_device_get_property_value(usb_tools->device, "ID_VENDOR",
        &con_data_usb->vendor_name);
    sd_device_get_property_value(usb_tools->device, "ID_MODEL_ID",
        &con_data_usb->product_id);
    sd_device_get_property_value(usb_tools->device, "ID_MODEL",
        &con_data_usb->product_name);
    sd_device_get_property_value(usb_tools->device, "DEVNAME",
        &con_data_usb->path_usb);
}

static void init_struct_unknown_temp_data_usb(temp_data_usb_t *unknown)
{
    unknown->vendor_id = NULL;
    unknown->vendor_name = strdup("Unknown");
    unknown->product_id = NULL;
    unknown->product_name = strdup("Unknown");
}

static void free_unknown_temp_data_usb(temp_data_usb_t *unknown)
{
    free(unknown->vendor_name);
    free(unknown->product_name);
}

static void display_couple_vendor_product_device(con_data_usb_t *con_data_usb,
    temp_data_usb_t *temp_data_usb, usb_risk_t *usb_risk)
{
    printf(
        "\e[1;37m╭───────────────────────────────────────────────── Device n°""\e[1;32m%lu\e[0m ""\e[1;37m─────────────────────────────────────────────────╮\e[0m\n"
        "│ VendorID  (\e[1;32m%s\e[0m)   │   ProductID (\e[1;32m%s\e[0m)\n"
        "│\n"
        "│ \e[1;36mFrom System\e[0m:\n"
        "│     Vendor Name (\e[1;34m%s\e[0m)   │   Product Name (\e[1;34m%s\e[0m)\n"
        "│\n"
        "│ \e[1;36mFrom Database\e[0m:\n"
        "│     Vendor Name (\e[1;34m%s\e[0m)   │   Product Name (\e[1;34m%s\e[0m)\n"
        "│\n"
        "│ Path : %s\n"
        "\e[1;37m╰────────────────────────────────────────────────────────────────────────────────────────────────────────────╯\e[0m\n\n",
        usb_risk->seen_count,
        con_data_usb->vendor_id,
        con_data_usb->product_id,
        con_data_usb->vendor_name,
        con_data_usb->product_name,
        temp_data_usb->vendor_name,
        temp_data_usb->product_name,
        con_data_usb->path_usb);
    ++usb_risk->low;
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
    return ERROR;
}

static void display_vendor_found_product_unknown_device(con_data_usb_t *con_data_usb,
    temp_data_usb_t *temp_data_usb, usb_risk_t *usb_risk)
{
    
    printf(
        "\e[1;37m╭───────────────────────────────────────────────── Device n°""\e[1;33m%lu\e[0m ""\e[1;37m─────────────────────────────────────────────────╮\e[0m\n"
        "│ VendorID  (\e[1;32m%s\e[0m)   │   ProductID (\e[1;31mUnknown : %s\e[0m)\n"
        "│\n"
        "│ \e[1;36mFrom System\e[0m:\n"
        "│     Vendor Name (\e[1;34m%s\e[0m)   │   Product Name (\e[1;34m%s\e[0m)\n"
        "│\n"
        "│ \e[1;36mFrom Database\e[0m:\n"
        "│     Vendor Name (\e[1;31m%s\e[0m)   │   Product Name (\e[1;31m%s\e[0m)\n"
        "│\n"
        "│ Path : %s\n"
        "\e[1;37m╰────────────────────────────────────────────────────────────────────────────────────────────────────────────╯\e[0m\n\n",
        usb_risk->seen_count,
        con_data_usb->vendor_id,
        con_data_usb->product_id,
        con_data_usb->vendor_name,
        con_data_usb->product_name,
        temp_data_usb->vendor_name,
        temp_data_usb->product_name,
        con_data_usb->path_usb);
    ++usb_risk->medium;
}

static void display_vendor_unknown_product_unknown_device(con_data_usb_t *con_data_usb,
    temp_data_usb_t *temp_data_usb, usb_risk_t *usb_risk)
{
    
    printf(
        "\e[1;37m╭───────────────────────────────────────────────── Device n°""\e[1;31m%lu\e[0m ""\e[1;37m─────────────────────────────────────────────────╮\e[0m\n"
        "│ VendorID  \e[1;31m(Unknown : %s\e[0m)   │   ProductID \e[1;31m(Unknown : %s\e[0m)\n"
        "│\n"
        "│ \e[1;36mFrom System\e[0m:\n"
        "│     Vendor Name (\e[1;31m%s\e[0m)   │   Product Name (\e[1;31m%s\e[0m)\n"
        "│\n"
        "│ \e[1;36mFrom Database\e[0m:\n"
        "│     Vendor Name (\e[1;31m%s\e[0m)   │   Product Name (\e[1;31m%s\e[0m)\n"
        "│\n"
        "│ Path : %s\n"
        "\e[1;37m╰────────────────────────────────────────────────────────────────────────────────────────────────────────────╯\e[0m\n\n",
        usb_risk->seen_count,
        con_data_usb->vendor_id,
        con_data_usb->product_id,
        con_data_usb->vendor_name,
        con_data_usb->product_name,
        temp_data_usb->vendor_name,
        temp_data_usb->product_name,
        con_data_usb->path_usb);
    ++usb_risk->major;
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

static void free_usb_db(usb_db_t *usb_db)
{
    for (size_t i = 0; i < usb_db->count; i++) {
        free(usb_db->entries[i].vendor_id);
        free(usb_db->entries[i].vendor_name);
        free(usb_db->entries[i].product_id);
        free(usb_db->entries[i].product_name);
    }
    free(usb_db->entries);
}

static void display_risk_table(usb_risk_t *usb_risk)
{
     printf(
        "\e[1;37m╭───────── Risk table ─────────╮\e[0m\n"
        "│ Number Low Risk    :  \e[1;32m%lu\e[0m \n"
        "│\n"
        "│ Number Medium Risk :  \e[1;33m%lu\e[0m \n"
        "│\n"
        "│ Number Major Risk  :  \e[1;31m%lu\e[0m \n"
        "\e[1;37m╰─────────────────────────────╯\e[0m\n\n",
    usb_risk->low, usb_risk->medium, usb_risk->major);

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
