/**
 * @name druid
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file fill_data_file.c
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

static void fill_struct_temp_data(temp_data_usb_t *temp_data_usb, char *line)
{
    char *vendor_id = strtok(line, FILE_SEPARATOR);
    char *vendor_name = strtok(NULL, FILE_SEPARATOR);
    char *product_id = strtok(NULL, FILE_SEPARATOR);
    char *product_name = strtok(NULL, FILE_SEPARATOR);

    remove_newline(product_name);
    temp_data_usb->vendor_id = strdup(vendor_id);
    temp_data_usb->vendor_name = strdup(vendor_name);
    temp_data_usb->product_id = strdup(product_id);
    temp_data_usb->product_name = strdup(product_name);
}

static int add_entry_to_usb_db(usb_db_t *usb_db, temp_data_usb_t **temp_data_usb,
    char *line, size_t *mem_allocated)
{
    if (usb_db->count >= *mem_allocated) {
        *mem_allocated *= INCREASED_SIZE;
        usb_db->entries = realloc(usb_db->entries, sizeof(temp_data_usb_t) * (*mem_allocated));
        if (usb_db->entries == NULL)
            return MAJOR_ERROR;
    }
    *temp_data_usb = &usb_db->entries[usb_db->count];
    init_struct_temp_data(*temp_data_usb);
    fill_struct_temp_data(*temp_data_usb, line);
    ++usb_db->count;
    return EXIT_SUCCESS;
}

static int add_new_data(param_t *param, usb_db_t *usb_db,
    temp_data_usb_t *temp_data_usb, size_t mem_allocated)
{
    char *line = NULL;
    size_t n = 0;
    FILE *update_data_file = fopen(strcat(param->av[PARAM_FILE], FILE_TYPE_PLUS_SEPARATOR), READ_MODE);

    if (update_data_file == NULL)
        return MAJOR_ERROR;
    while (getline(&line, &n, update_data_file) != EOF) {
        if (add_entry_to_usb_db(usb_db, &temp_data_usb, line, &mem_allocated) == MAJOR_ERROR)
            return MAJOR_ERROR;
    }
    fclose(update_data_file);
    free(line);
    return EXIT_SUCCESS;
}

static int check_update_file(param_t *param, usb_db_t *usb_db,
    temp_data_usb_t *temp_data_usb, size_t mem_allocated)
{
    if (param->ac == BIN_FLAG_FILE &&
        (strcmp(param->av[PARAM_FLAG], UPDATE_FLAG) == SUCCESS
        || strcmp(param->av[PARAM_FLAG], UPDATE_FLAG_OPTION) == SUCCESS)) {
            strtok(param->av[PARAM_FILE], FILE_TYPE_SEPARATOR);
            if (strcmp(strtok(NULL, FILE_TYPE_SEPARATOR), FILE_TYPE) != SUCCESS) {
                printf(NONE_CSV_FILE_MESSAGE);
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

int fill_struct_db_temp(usb_db_t *usb_db, temp_data_usb_t *temp_data_usb, param_t *param)
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
        if (add_entry_to_usb_db(usb_db, &temp_data_usb, line, &mem_allocated) == MAJOR_ERROR)
            return MAJOR_ERROR;
    }
    free(line);
    fclose(data_file);
    return EXIT_SUCCESS;
}
