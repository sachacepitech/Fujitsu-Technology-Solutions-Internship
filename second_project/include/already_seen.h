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

    #define FILE_SEPARATOR ";"

    #include <stddef.h>
    #include <systemd/sd-device.h>


typedef struct seen_device_s {
    const char *vendor_id;
    const char *product_id;
} seen_device_t;

    #define MAX_SEEN_DEVICES 128

seen_device_t seen[MAX_SEEN_DEVICES];

#endif /* SEEN_H */
