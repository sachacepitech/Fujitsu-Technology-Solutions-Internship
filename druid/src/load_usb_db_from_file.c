/**
 * @name druid
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file load_usb_db_from_file.c
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

static void remove_newline(char *str)
{
    size_t len = strlen(str);

    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

static void fill_struct_temp_data(usb_db_entry_t *usb_db_entry, char *line)
{
    char *vendor_id = strtok(line, FILE_SEPARATOR);
    char *vendor_name = strtok(NULL, FILE_SEPARATOR);
    char *product_id = strtok(NULL, FILE_SEPARATOR);
    char *product_name = strtok(NULL, FILE_SEPARATOR);

    remove_newline(product_name);
    usb_db_entry->vendor_id = strdup(vendor_id);
    usb_db_entry->vendor_name = strdup(vendor_name);
    usb_db_entry->product_id = strdup(product_id);
    usb_db_entry->product_name = strdup(product_name);
}

static int append_usb_entry_from_line(usb_db_t *usb_db, usb_db_entry_t **usb_db_entry,
    char *line, size_t *allocated_capacity)
{
    if (usb_db->count >= *allocated_capacity) {
        *allocated_capacity *= INCREASED_SIZE;
        usb_db->entries = realloc(usb_db->entries, sizeof(usb_db_entry_t) * (*allocated_capacity));
        if (usb_db->entries == NULL)
            return MAJOR_ERROR;
    }
    *usb_db_entry = &usb_db->entries[usb_db->count];
    init_struct_temp_data(*usb_db_entry);
    fill_struct_temp_data(*usb_db_entry, line);
    ++usb_db->count;
    return EXIT_SUCCESS;
}

static int add_new_data(cli_args_t *cli_args, usb_db_t *usb_db,
    usb_db_entry_t *usb_db_entry, size_t allocated_capacity)
{
    char *line = NULL;
    size_t n = 0;
    FILE *update_data_file = fopen(strcat(cli_args->av[cli_args_FILE], FILE_TYPE_PLUS_SEPARATOR), READ_MODE);

    if (update_data_file == NULL)
        return MAJOR_ERROR;
    while (getline(&line, &n, update_data_file) != EOF) {
        if (append_usb_entry_from_line(usb_db, &usb_db_entry, line, &allocated_capacity) == MAJOR_ERROR)
            return MAJOR_ERROR;
    }
    fclose(update_data_file);
    free(line);
    return EXIT_SUCCESS;
}

static int check_for_update_file_and_load(cli_args_t *cli_args, usb_db_t *usb_db,
    usb_db_entry_t *usb_db_entry, size_t allocated_capacity)
{
    if (cli_args->ac == BIN_FLAG_FILE &&
        (strcmp(cli_args->av[cli_args_FLAG], UPDATE_FLAG) == SUCCESS
        || strcmp(cli_args->av[cli_args_FLAG], UPDATE_FLAG_OPTION) == SUCCESS)) {
            strtok(cli_args->av[cli_args_FILE], FILE_TYPE_SEPARATOR);
            if (strcmp(strtok(NULL, FILE_TYPE_SEPARATOR), FILE_TYPE) != SUCCESS) {
                printf(NONE_CSV_FILE_MESSAGE);
                return MAJOR_ERROR;
            } else {
                if (add_new_data(cli_args, usb_db,
                    usb_db_entry, allocated_capacity) == MAJOR_ERROR)
                    return MAJOR_ERROR;
            }
    } else
        return EXIT_SUCCESS;
    return EXIT_SUCCESS;
}

int load_usb_db_from_file(usb_db_t *usb_db, usb_db_entry_t *usb_db_entry, cli_args_t *cli_args)
{
    FILE *data_file = fopen(DATA_FILE_PATH, READ_MODE);
    char *line = NULL;
    size_t n = 0;
    size_t allocated_capacity = DEFAULT_SIZE;

    if (data_file == NULL)
        return MAJOR_ERROR;
    if (init_struct_usb_db(usb_db, allocated_capacity) == MAJOR_ERROR)
        return MAJOR_ERROR;
    if (check_for_update_file_and_load(cli_args, usb_db, usb_db_entry, allocated_capacity) == MAJOR_ERROR)
        return MAJOR_ERROR;
    while (getline(&line, &n, data_file) != EOF) {
        if (append_usb_entry_from_line(usb_db, &usb_db_entry, line, &allocated_capacity) == MAJOR_ERROR)
            return MAJOR_ERROR;
    }
    free(line);
    fclose(data_file);
    return EXIT_SUCCESS;
}
