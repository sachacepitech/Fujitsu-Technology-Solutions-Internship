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

static void init_temp_data_struct(temp_data_usb_t *temp_data_usb)
{
    temp_data_usb->vendor_id = NULL;
    temp_data_usb->vendor_name = NULL;
    temp_data_usb->product_id = NULL;
    temp_data_usb->product_name = NULL;
}

static void init_usb_db_struct(usb_db_t *usb_db, size_t mem_allocated)
{
    usb_db->entries = malloc(sizeof(temp_data_usb_t) * mem_allocated);
    usb_db->count = 0;
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

static void fill_temp_data_struct(temp_data_usb_t *temp_data_usb, char *line)
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

void get_device_values(usb_tools_t *usb_tools,
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
}

static void display_known_device(con_data_usb_t *con_data_usb,
    temp_data_usb_t *temp_data_usb, unsigned int device_count)
{
    printf("Device n°%u ID/NAME:\n"
           "    VendorID (\e[1;32m%s\e[0m) / ProductID (\e[1;32m%s\e[0m)\n"
           "    From System:\tvendorID (\e[1;32m%s\e[0m) \t/\t productID (\e[1;32m%s\e[0m)\n"
           "    From Dtabase:\tvendorID (\e[1;32m%s\e[0m) \t/\t productID (\e[1;32m%s\e[0m)\n"
           "-----------------------------------------------"
           "--------------------------------------------------\n",
        device_count,
        con_data_usb->vendor_id, con_data_usb->product_id,
        con_data_usb->vendor_name, temp_data_usb->vendor_name,
        con_data_usb->product_name, temp_data_usb->product_name);
}

static int load_data_file_n_reader(usb_db_t *usb_db,
    temp_data_usb_t *temp_data_usb)
{
    FILE *data_file = fopen(DATA_FILE_PATH, "r");
    char *line = NULL;
    size_t n = 0;
    size_t mem_allocated = DEFAULT_SIZE;

    init_usb_db_struct(usb_db, mem_allocated);
    while (getline(&line, &n, data_file) != EOF) {
        if (usb_db->count >= mem_allocated) {
            mem_allocated *= INCREASED_SIZE;
            usb_db->entries = realloc(usb_db->entries, sizeof(temp_data_usb_t)
                * mem_allocated);
        }
        temp_data_usb = &usb_db->entries[usb_db->count];
        init_temp_data_struct(temp_data_usb);
        fill_temp_data_struct(temp_data_usb, line);
        ++usb_db->count;
    }
    free(line);
    fclose(data_file);
    return EXIT_SUCCESS;
}

int comparator(usb_tools_t *usb_tools, con_data_usb_t *con_data_usb,
    temp_data_usb_t *temp_data_usb)
{
    usb_db_t usb_db = {0};
    size_t seen_count = 0;
    bool already_seen = false;

    if (load_data_file_n_reader(&usb_db, temp_data_usb) == EXIT_FAILURE)
        return EXIT_FAILURE;
    usb_tools->device = sd_device_enumerator_get_device_first(
        usb_tools->enumerator);
    while (usb_tools->device != NULL) {
        already_seen = false;
        get_device_values(usb_tools, con_data_usb);
        for (size_t k = 0; k < seen_count; ++k) {
            if (strcmp(con_data_usb->vendor_id, seen[k].vendor_id) == 0 &&
                strcmp(con_data_usb->product_id, seen[k].product_id) == 0) {
                already_seen = true;
                break;
            }
        }
        if (already_seen == true) {
            usb_tools->device = sd_device_enumerator_get_device_next(
                usb_tools->enumerator);
            continue;
        }
        for (size_t i = 0; i < usb_db.count; ++i) {
            temp_data_usb = &usb_db.entries[i];
            if (strcmp(con_data_usb->vendor_id, temp_data_usb->vendor_id) == 0 &&
                strcmp(con_data_usb->product_id, temp_data_usb->product_id) == 0) {
                display_known_device(con_data_usb, temp_data_usb, seen_count);
                if (seen_count < MAX_SEEN_DEVICES) {
                    seen[seen_count].vendor_id = con_data_usb->vendor_id;
                    seen[seen_count].product_id = con_data_usb->product_id;
                    ++seen_count;
                }
                break;
            }
        }
        usb_tools->device = sd_device_enumerator_get_device_next(
            usb_tools->enumerator);
    }
    free_usb_db(&usb_db);
    return EXIT_SUCCESS;
}
