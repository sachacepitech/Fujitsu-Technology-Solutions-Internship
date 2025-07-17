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

int browse_device(sd_device *device, sd_device_enumerator *enumerator,
    con_data_usb_t *con_data_usb, temp_data_usb_t *temp_data_usb)
{
    while (device != NULL) {
        sd_device_get_property_value(device, "ID_VENDOR_ID",
            &con_data_usb->vendor_id);
        sd_device_get_property_value(device, "ID_VENDOR",
            &con_data_usb->vendor_name);
        sd_device_get_property_value(device, "ID_MODEL_ID",
            &con_data_usb->product_id);
        sd_device_get_property_value(device, "ID_MODEL",
            &con_data_usb->product_name);
        if (strcmp(con_data_usb->vendor_id, temp_data_usb->vendor_id) == 0
            /*|| strcmp(con_data_usb->product_id, temp_data_usb->vendor_id) == 0*/) {
            printf("vendor ID/NAME/PRODUCT:\n"
                "   vendor id (from device/ from data) : %s/%s\n"
                "   vendor name (from device/ from data): %s/%s\n"
                "   product name (from device/ from data): %s/%s"
                "-------------------------------------------------\n",
                con_data_usb->vendor_id, temp_data_usb->vendor_id,
                con_data_usb->vendor_name, temp_data_usb->vendor_name, 
                con_data_usb->product_name, temp_data_usb->product_name);
        }
        device = sd_device_enumerator_get_device_next(enumerator);
    }
    return EXIT_SUCCESS;
}
