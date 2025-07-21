/*
** FUJITSU PROJECT, 2025
** second_project
** Version:
** 1.0
** File description:
** dependencies_project.h
** Author:
** Sacha Lemée, @sacha-lemee on Linkedin
** Junior Cybersecurity Consultant
** Created:
** ???
*/

#ifndef DEPENDENCIES_H
    #define DEPENDENCIES_H

    #define SUCCESS 0
    #define ERROR -1
    #define MAJOR_ERROR 84

    #define FILE_SEPARATOR ";"
    #define DATA_FILE_PATH "data-files/vendor_id_product_id_and_name.csv"
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

int create_enumerator(usb_tools_t *usb_tools, con_data_usb_t *con_data_usb);
int comparator(usb_tools_t *usb_tools, con_data_usb_t *con_data_usb,
    temp_data_usb_t *temp_data_usb);

#endif /* DEPENDENCIES_H */
