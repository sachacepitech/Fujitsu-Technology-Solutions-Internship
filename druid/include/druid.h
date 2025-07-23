/**
 * @name druid
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file druid.h
 * @date 17 July 2025
 * @copyright Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0)
 * 
 * This file is part of the "druid" repository.
 * 
 * You can use, modify, and distribute this code under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0).
 * See the full license at: https://creativecommons.org/licenses/by-sa/4.0/deed.fr
 */

#ifndef druid_H
    #define druid_H

    #define SUCCESS 0
    #define UNSEEN -1
    #define MAJOR_ERROR 84

    #define FILE_SEPARATOR ";"
    #define FILE_TYPE_SEPARATOR "."
    #define FILE_TYPE "csv"
    #define FILE_TYPE_PLUS_SEPARATOR ".csv"
    #define READ_MODE "r"
    
    #define DATA_FILE_PATH "data-files/vendor_id_product_id_and_name.csv"
    
    #define HELP_FILE "src/INFO_FILE/HELP"
    #define FORMAT_FILE "src/INFO_FILE/FORMAT"
    #define LICENSE_FILE "src/INFO_FILE/LICENSE"

    #define VENDOR_ID "ID_VENDOR_ID"
    #define VENDOR_NAME "ID_VENDOR"
    #define PRODUCT_ID "ID_MODEL_ID"
    #define PRODUCT_NAME "ID_MODEL"

    #define HELP_FLAG "-h"
    #define FORMAT_FLAG "-f"
    #define LICENSE_FLAG "-l"
    #define UPDATE_FLAG "-u"

    #define HELP_FLAG_OPTION "--help"
    #define FORMAT_FLAG_OPTION "--format"
    #define LICENSE_FLAG_OPTION "--license"
    #define UPDATE_FLAG_OPTION "--update"

    #define UNKNOWN_DEVICE_MESSAGE "Unknown"
    #define NONE_CSV_FILE_MESSAGE "Error: Make sure it is a csv file.\n"

    #define cli_args_FLAG 1
    #define cli_args_FILE 2
    #define BIN_FLAG_FILE 3

    #include <stddef.h>
    #include <systemd/sd-device.h>

typedef struct usb_db_entry_s {
    char *vendor_id;
    char *vendor_name;
    char *product_id;
    char *product_name;
} usb_db_entry_t;

typedef struct data_usb_con_s {
    const char *vendor_id;
    const char *vendor_name;
    const char *product_id;
    const char *product_name;
    const char *path_usb;
} usb_device_info_t;

typedef struct usb_tools_s {
    sd_device *device;
    sd_device_enumerator *enumerator;
} usb_tools_t;

    #define DEFAULT_SIZE 10
    #define INCREASED_SIZE 2

typedef struct usb_db_s {
    usb_db_entry_t *entries;
    size_t count;
} usb_db_t;

typedef struct usb_risk_stats_s {
    size_t low;
    size_t medium;
    size_t major;
    size_t seen_count;
} usb_risk_stats_stats_t;

typedef struct cli_args_s {
    int ac;
    char **av;
} cli_args_t;

/* init all struct */
void init_usb_tools_struct(usb_tools_t *usb_tools);
void init_usb_device_info_struct(usb_device_info_t *usb_device_info);
int init_struct_usb_db(usb_db_t *usb_db, size_t allocated_capacity);
void init_struct_temp_data(usb_db_entry_t *usb_db_entry);
void init_struct_unknown_usb_db_entry(usb_db_entry_t *unknown);

/* fill database struct */
int load_usb_db_from_file(usb_db_t *usb_db, usb_db_entry_t *usb_db_entry, cli_args_t *cli_args);

/* free all */
void free_unknown_usb_db_entry(usb_db_entry_t *unknown);
void free_usb_db(usb_db_t *usb_db);

/* display risk case*/
void display_known_usb_device(usb_device_info_t *usb_device_info,
    usb_db_entry_t *usb_db_entry, usb_risk_stats_stats_t *usb_risk_stats);
void display_partially_known_usb_device(usb_device_info_t *usb_device_info,
    usb_db_entry_t *usb_db_entry, usb_risk_stats_stats_t *usb_risk_stats);
void display_unknown_usb_device(usb_device_info_t *usb_device_info,
    usb_db_entry_t *usb_db_entry, usb_risk_stats_stats_t *usb_risk_stats);
void display_risk_table(usb_risk_stats_stats_t *usb_risk_stats);

/* option */
int handle_cli_info_flags(int ac, char **av);
int display_file(int ac, char **av, const char *flag,
    const char *optional_flag, const char *path_file);

int init_usb_enumerator(usb_tools_t *usb_tools, usb_device_info_t *usb_device_info);
int scan_connected_usb_and_check_risks(usb_tools_t *usb_tools, usb_device_info_t *usb_device_info,
    usb_db_entry_t *usb_db_entry, cli_args_t *cli_args);

#endif /* druid_H */
