/*
** FUJITSU PROJECT, 2025
** second_project
** Version:
** 1.0
** File description:
** browse_device.c
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

static void display_known_device(con_data_usb_t *con_data_usb,
    temp_data_usb_t *temp_data_usb, unsigned int device_count)
{
    printf("Device n°%u ID/NAME:\n"
        "\tvendor id (from device / from data): %s/%s\n"
        "\tproduct id (from device / from data): %s/%s\n"
        "\tvendor name (from device / from data): %s/%s\n"
        "\tproduct name (from device / from data): %s/%s"
        "-----------------------------------------------"
        "--------------------------------------------------\n",
        device_count,
        con_data_usb->vendor_id, temp_data_usb->vendor_id,
        con_data_usb->product_id, temp_data_usb->product_id,
        con_data_usb->vendor_name, temp_data_usb->vendor_name,
        con_data_usb->product_name, temp_data_usb->product_name);
}

static void get_device_values(usb_tools_t *usb_tools,
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

int browse_device(usb_tools_t *usb_tools, con_data_usb_t *con_data_usb,
    temp_data_usb_t *temp_data_usb)
{
    static unsigned int device_count = 0;

    while (usb_tools->device != NULL) {
        get_device_values(usb_tools, con_data_usb);
        if (strcmp(con_data_usb->vendor_id, temp_data_usb->vendor_id) == 0
            && strcmp(con_data_usb->product_id, temp_data_usb->product_id)
            == 0) {
            ++device_count;
            display_known_device(con_data_usb, temp_data_usb, device_count);
        }
        usb_tools->device = sd_device_enumerator_get_device_next(
            usb_tools->enumerator);
    }
    return EXIT_SUCCESS;
}
