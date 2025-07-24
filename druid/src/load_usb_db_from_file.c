/**
 * @name druid
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file load_usb_db_from_file.c
 * @brief Load usb database from file(s)
 * @details Load and parse usb db from file(s)
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

/**
 * @brief Removes the trailing newline character from a string
 *
 * checks if the last character of the string is a newline ('\n')
 * and replaces it with a null terminator to clean the string
 * 
 * @details static void remove_newline(char *str)
 * @param str Pointer to the null-terminated string to modify
 * @return None (void)
 */
static void remove_newline(char *str)
{
    size_t len = strlen(str);

    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

/**
 * @brief Parses a CSV line and fills a USB database entry structure
 *
 * splits the input line using the defined separator to extract vendor ID, vendor name,
 * product ID, and product name, then duplicates and assigns these strings
 * to the corresponding fields in the USB database entry
 * 
 * @details static void fill_struct_temp_data(
 *             usb_db_entry_t *usb_db_entry,
 *             char *line)
 * @param usb_db_entry Pointer to the usb_db_entry_t structure to fill
 * @param line Input CSV formatted string containing USB device data
 * @return None (void)
 */
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

/**
 * @brief Appends a USB database entry parsed from a single line
 *
 * checks if the USB database array needs resizing and reallocates if necessary,
 * then initializes and fills a new database entry with parsed data from the line,
 * and increments the entry count
 * 
 * @details static int append_usb_entry_from_line(
 *             usb_db_t *usb_db,
 *             usb_db_entry_t **usb_db_entry,
 *             char *line,
 *             size_t *allocated_capacity)
 * @param usb_db Pointer to the usb_db_t structure holding the database entries
 * @param usb_db_entry Double pointer to a usb_db_entry_t structure to update with new entry
 * @param line String containing the raw database entry line to parse
 * @param allocated_capacity Pointer to the current allocated capacity of the database array
 * @return Exit code:
 *         - 0      (EXIT_SUCCESS) on successful append
 *         - 84     (EXIT_ERROR) if memory reallocation fails
 */
static int append_usb_entry_from_line(usb_db_t *usb_db, usb_db_entry_t **usb_db_entry,
    char *line, size_t *allocated_capacity)
{
    if (usb_db->count >= *allocated_capacity) {
        *allocated_capacity *= INCREASED_SIZE;
        usb_db->entries = realloc(usb_db->entries, sizeof(usb_db_entry_t) * (*allocated_capacity));
        if (usb_db->entries == NULL)
            return EXIT_ERROR;
    }
    *usb_db_entry = &usb_db->entries[usb_db->count];
    init_struct_usb_db_entry(*usb_db_entry);
    fill_struct_temp_data(*usb_db_entry, line);
    ++usb_db->count;
    return EXIT_SUCCESS;
}

/**
 * @brief Loads new USB database entries from an update file
 *
 * opens the specified update file, reads its content line by line,
 * and appends each entry to the existing USB database
 * 
 * @details static int add_new_data(
 *             cli_args_t *cli_args,
 *             usb_db_t *usb_db,
 *             usb_db_entry_t *usb_db_entry,
 *             size_t allocated_capacity)
 * @param cli_args Pointer to the cli_args_t structure containing CLI arguments
 * @param usb_db Pointer to the usb_db_t structure to append new entries
 * @param usb_db_entry Pointer to the usb_db_entry_t structure used during parsing
 * @param allocated_capacity Initial allocated size for database entries
 * @return Exit code:
 *         - 0      (EXIT_SUCCESS) on successful loading and appending
 *         - 84     (EXIT_ERROR) if the file cannot be opened or reading fails
 */
static int add_new_data(cli_args_t *cli_args, usb_db_t *usb_db,
    usb_db_entry_t *usb_db_entry, size_t allocated_capacity)
{
    char *line = NULL;
    size_t n = 0;
    FILE *update_data_file = fopen(strcat(cli_args->av[cli_args_FILE], FILE_TYPE_PLUS_SEPARATOR), READ_MODE);

    if (update_data_file == NULL)
        return EXIT_ERROR;
    while (getline(&line, &n, update_data_file) != EOF) {
        if (append_usb_entry_from_line(usb_db, &usb_db_entry, line, &allocated_capacity) == EXIT_ERROR)
            return EXIT_ERROR;
    }
    fclose(update_data_file);
    free(line);
    return EXIT_SUCCESS;
}

/**
 * @brief Checks for a file update request via CLI and loads new data if valid
 *
 * verifies if the CLI input requests an update, ensures the file format is correct,
 * and triggers the loading of new USB database entries if applicable
 * 
 * @details static int check_for_update_file_and_load(
 *             cli_args_t *cli_args,
 *             usb_db_t *usb_db,
 *             usb_db_entry_t *usb_db_entry,
 *             size_t allocated_capacity)
 * @param cli_args Pointer to the cli_args_t structure containing CLI arguments
 * @param usb_db Pointer to the usb_db_t structure to append new entries
 * @param usb_db_entry Pointer to the usb_db_entry_t structure used during parsing
 * @param allocated_capacity Initial allocated size for database entries
 * @return Exit code:
 *         - 0      (EXIT_SUCCESS) if no update is requested or update is successful
 *         - 84     (EXIT_ERROR) if the file is invalid or loading fails
 */
static int check_for_update_file_and_load(cli_args_t *cli_args, usb_db_t *usb_db,
    usb_db_entry_t *usb_db_entry, size_t allocated_capacity)
{
    if (cli_args->ac == BIN_FLAG_FILE &&
        (strcmp(cli_args->av[cli_args_FLAG], UPDATE_FLAG) == SUCCESS
        || strcmp(cli_args->av[cli_args_FLAG], UPDATE_FLAG_OPTION) == SUCCESS)) {
            strtok(cli_args->av[cli_args_FILE], FILE_TYPE_SEPARATOR);
            if (strcmp(strtok(NULL, FILE_TYPE_SEPARATOR), FILE_TYPE) != SUCCESS) {
                printf(NONE_CSV_FILE_MESSAGE);
                return EXIT_ERROR;
            } else {
                if (add_new_data(cli_args, usb_db,
                    usb_db_entry, allocated_capacity) == EXIT_ERROR)
                    return EXIT_ERROR;
            }
    } else
        return EXIT_SUCCESS;
    return EXIT_SUCCESS;
}

/**
 * @brief Loads USB device data from the local database file
 *
 * opens the USB data file, initializes the database structure,
 * checks for file updates, and appends entries line by line
 * 
 * @details int load_usb_db_from_file(
 *             usb_db_t *usb_db,
 *             usb_db_entry_t *usb_db_entry,
 *             cli_args_t *cli_args)
 * @param usb_db Pointer to the usb_db_t structure to populate with entries
 * @param usb_db_entry Pointer to a usb_db_entry_t structure used during parsing
 * @param cli_args Pointer to the cli_args_t structure containing CLI arguments
 * @return Exit code:
 *         - 0      (EXIT_SUCCESS) if the file was successfully loaded
 *         - 84     (EXIT_ERROR) on failure (file missing, allocation error, etc.)
 */
int load_usb_db_from_file(usb_db_t *usb_db, usb_db_entry_t *usb_db_entry, cli_args_t *cli_args)
{
    FILE *data_file = fopen(DATA_FILE_PATH, READ_MODE);
    char *line = NULL;
    size_t n = 0;
    size_t allocated_capacity = DEFAULT_SIZE;

    if (data_file == NULL)
        return EXIT_ERROR;
    if (init_struct_usb_db(usb_db, allocated_capacity) == EXIT_ERROR)
        return EXIT_ERROR;
    if (check_for_update_file_and_load(cli_args, usb_db, usb_db_entry, allocated_capacity) == EXIT_ERROR)
        return EXIT_ERROR;
    while (getline(&line, &n, data_file) != EOF) {
        if (append_usb_entry_from_line(usb_db, &usb_db_entry, line, &allocated_capacity) == EXIT_ERROR)
            return EXIT_ERROR;
    }
    free(line);
    fclose(data_file);
    return EXIT_SUCCESS;
}
