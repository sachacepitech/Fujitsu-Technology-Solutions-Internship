/**
 * @name second project
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file dependencies_project.h
 * @date 17 July 2025
 * @copyright Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0)
 * 
 * This file is part of the "second project" repository.
 * 
 * You can use, modify, and distribute this code under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0).
 * See the full license at: https://creativecommons.org/licenses/by-sa/4.0/deed.fr
 */

#ifndef DEPENDENCIES_H
    #define DEPENDENCIES_H

    #define SUCCESS 0
    #define ERROR -1
    #define MAJOR_ERROR 84

    #define FILE_SEPARATOR ";"
    #define FILE_TYPE "csv"
    #define DATA_FILE_PATH "data-files/vendor_id_product_id_and_name.csv"
    #define HELP_FILE "src/INFO_FILE/HELP"
    #define FORMAT_FILE "src/INFO_FILE/FORMAT"
    #define LICENSE_FILE "src/INFO_FILE/LICENSE"
    #define READ_MODE "r"

    #include <stddef.h>
    #include <systemd/sd-device.h>

typedef struct temp_data_usb_s {
    char *vendor_id;
    char *vendor_name;
    char *product_id;
    char *product_name;
} temp_data_usb_t;

typedef struct data_usb_con_s {
    const char *vendor_id;
    const char *vendor_name;
    const char *product_id;
    const char *product_name;
    const char *path_usb;
} con_data_usb_t;

typedef struct usb_tools_s {
    sd_device *device;
    sd_device_enumerator *enumerator;
} usb_tools_t;

    #define DEFAULT_SIZE 10
    #define INCREASED_SIZE 2

typedef struct usb_db_s {
    temp_data_usb_t *entries;
    size_t count;
} usb_db_t;

typedef struct usb_risk_s {
    size_t low;
    size_t medium;
    size_t major;
    size_t seen_count;
} usb_risk_t;

typedef struct param_s {
    int ac;
    char **av;
} param_t;

int flags_files(int ac, char **av);
int display_file(int ac, char **av, const char *flag,
    const char *optional_flag, const char *path_file);
int create_enumerator(usb_tools_t *usb_tools, con_data_usb_t *con_data_usb);
int comparator(usb_tools_t *usb_tools, con_data_usb_t *con_data_usb,
    temp_data_usb_t *temp_data_usb, param_t *param);

#endif /* DEPENDENCIES_H */
