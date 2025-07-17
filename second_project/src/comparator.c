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
#include <stddef.h>
#include <string.h>

#include <systemd/sd-device.h>

#include "dependencies_project.h"

static void init_temp_data_struct(temp_data_usb_t *temp_data_usb)
{
    temp_data_usb->vendor_id = NULL;
    temp_data_usb->vendor_name = NULL;
    temp_data_usb->product_id = NULL;
    temp_data_usb->product_name = NULL;
    temp_data_usb->separator = FILE_SEPARATOR;
}

static int data_reader(FILE *data_file, temp_data_usb_t *temp_data_usb,
    usb_tools_t *usb_tools, con_data_usb_t *con_data_usb)
{
    char *line = NULL;
    size_t n = 0;

    init_temp_data_struct(temp_data_usb);
    while (getline(&line, &n, data_file) != EOF) {
        temp_data_usb->vendor_id = strtok(line, temp_data_usb->separator);
        temp_data_usb->vendor_name = strtok(NULL, temp_data_usb->separator);
        temp_data_usb->product_id = strtok(NULL, temp_data_usb->separator);
        temp_data_usb->product_name = strtok(NULL, temp_data_usb->separator);
        browse_device(usb_tools->device, usb_tools->enumerator,
            con_data_usb, temp_data_usb);
        usb_tools->device = sd_device_enumerator_get_device_first(
            usb_tools->enumerator);
    }
    free(line);
    return EXIT_SUCCESS;
}

int comparator(usb_tools_t *usb_tools, con_data_usb_t *con_data_usb)
{
    FILE *data_file = fopen(DATA_FILE_PATH, "r");
    temp_data_usb_t temp_data_usb = {0};

    if (data_reader(data_file, &temp_data_usb, usb_tools, con_data_usb) == EXIT_FAILURE)
        return EXIT_FAILURE;
    fclose(data_file);
    return EXIT_SUCCESS;
}