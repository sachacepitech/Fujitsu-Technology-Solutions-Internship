/*
** FUJITSU PROJECT, 2025
** second_project
** Version:
** 1.0
** File description:
** already_seen.h
** Author:
** Sacha Lemée, @sacha-lemee on Linkedin
** Junior Cybersecurity Consultant
** Created:
** ???
*/

#ifndef SEEN_H
    #define SEEN_H
    #define EXIT_SUCCESS 0

    #define FILE_SEPARATOR ";"
    #define DATA_FILE_PATH "data-files/vendor_id_product_id_and_name.csv"

    #include <stddef.h>
    #include <systemd/sd-device.h>


typedef struct seen_device_s {
    const char *vendor_id;
    const char *product_id;
} seen_device_t;

    #define MAX_SEEN_DEVICES 128

seen_device_t seen[MAX_SEEN_DEVICES];

#endif /* SEEN_H */
