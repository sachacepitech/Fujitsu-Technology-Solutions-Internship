/*
** FUJITSU PROJECT, 2025
** second_project
** Version:
** 1.0
** File description:
** comparator.c
** Author:
** Sacha Lemée, @sacha-lemee on Linkedin
** Junior Cybersecurity Consultant
** Created:
** ???
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include <systemd/sd-device.h>

#include "dependencies_project.h"
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
    char *product_id = strtok(line, FILE_SEPARATOR);
    char *product_name = strtok(NULL, FILE_SEPARATOR);
    char *vendor_id = strtok(NULL, FILE_SEPARATOR);
    char *vendor_name = strtok(NULL, FILE_SEPARATOR);

    remove_newline(vendor_name);
    temp_data_usb->product_id = product_id ? strdup(product_id) : NULL;
    temp_data_usb->product_name = product_name ? strdup(product_name) : NULL;
    temp_data_usb->vendor_id = vendor_id ? strdup(vendor_id) : NULL;
    temp_data_usb->vendor_name = vendor_name ? strdup(vendor_name) : NULL;
}

static int fill_struct_db_temp(usb_db_t *usb_db,
    temp_data_usb_t *temp_data_usb)
{
    FILE *data_file = fopen(DATA_FILE_PATH, READ_MODE);
    char *line = NULL;
    size_t n = 0;
    size_t mem_allocated = DEFAULT_SIZE;

    if (data_file == NULL)
        return MAJOR_ERROR;    
    if (init_struct_usb_db(usb_db, mem_allocated) == MAJOR_ERROR)
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

/*
La lib utilisé considère que le ID_MODEL_ID (par conséquent le product ID),
est en réalité
*/
static void get_vendor_product_device(usb_tools_t *usb_tools,
    con_data_usb_t *con_data_usb)
{
    sd_device_get_property_value(usb_tools->device, "ID_MODEL_ID",
        &con_data_usb->vendor_id);
    sd_device_get_property_value(usb_tools->device, "ID_MODEL",
        &con_data_usb->vendor_name);
    sd_device_get_property_value(usb_tools->device, "ID_VENDOR_ID",
        &con_data_usb->product_id);
    sd_device_get_property_value(usb_tools->device, "ID_VENDOR",
        &con_data_usb->product_name);
}

static void display_couple_vendor_product_device(con_data_usb_t *con_data_usb,
    temp_data_usb_t *temp_data_usb, unsigned int device_count)
{
    printf("Device n°%u ID/NAME:\n"
        "    VendorID (\e[1;32m%s\e[0m) / ProductID (\e[1;32m%s\e[0m)\n"
        "    From System:\tvendorNAME (\e[1;32m%s\e[0m) \t/\t productNAME (\e[1;32m%s\e[0m)\n"
        "    From Dtabase:\tvendorNAME (\e[1;32m%s\e[0m) \t/\t productNAME (\e[1;32m%s\e[0m)\n"
        "-----------------------------------------------"
        "--------------------------------------------------\n",
        device_count,
        con_data_usb->vendor_id, con_data_usb->product_id,
        con_data_usb->vendor_name, con_data_usb->product_name,
        temp_data_usb->vendor_name, temp_data_usb->product_name);
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

static void check_usb_exist(usb_db_t *usb_db, temp_data_usb_t *temp_data_usb,
    con_data_usb_t *con_data_usb, size_t *seen_count)
{
    for (size_t i = 0; i < usb_db->count; ++i) {
        temp_data_usb = &usb_db->entries[i];
        if (strcmp(con_data_usb->vendor_id, temp_data_usb->vendor_id) == SUCCESS &&
            strcmp(con_data_usb->product_id, temp_data_usb->product_id) == SUCCESS) {
            display_couple_vendor_product_device(con_data_usb, temp_data_usb, *seen_count);
            if (*seen_count < MAX_SEEN_DEVICES) {
                seen[*seen_count].vendor_id = con_data_usb->vendor_id;
                seen[*seen_count].product_id = con_data_usb->product_id;
                ++(*seen_count);
            }
            break;
        }
    }
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

int comparator(usb_tools_t *usb_tools, con_data_usb_t *con_data_usb,
    temp_data_usb_t *temp_data_usb)
{
    usb_db_t usb_db = {0};
    size_t seen_count = 0;
    bool already_seen = false;

    if (fill_struct_db_temp(&usb_db, temp_data_usb) == MAJOR_ERROR)
        return MAJOR_ERROR;
    usb_tools->device = sd_device_enumerator_get_device_first(
        usb_tools->enumerator);
    while (usb_tools->device != NULL) {
        already_seen = false;
        get_vendor_product_device(usb_tools, con_data_usb);
        if (check_already_seen(usb_tools, con_data_usb, seen_count, already_seen) == SUCCESS)
            continue;
        check_usb_exist(&usb_db, temp_data_usb, con_data_usb, &seen_count);
        usb_tools->device = sd_device_enumerator_get_device_next(
            usb_tools->enumerator);
    }
    free_usb_db(&usb_db);
    return EXIT_SUCCESS;
}
