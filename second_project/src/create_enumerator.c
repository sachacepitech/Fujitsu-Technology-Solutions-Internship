/*
** FUJITSU PROJECT, 2025
** second_project
** Version:
** 1.0
** File description:
** create_enumerator.c
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

#include <systemd/sd-device.h>

#include "dependencies_project.h"

static void init_usb_tools_struct(usb_tools_t *usb_tools)
{
    usb_tools->device = NULL;
    usb_tools->enumerator = NULL;
}

static void init_con_data_usb_struct(con_data_usb_t *con_data_usb)
{
    con_data_usb->vendor_id = NULL;
    con_data_usb->vendor_name = NULL;
    con_data_usb->product_id = NULL;
    con_data_usb->product_name = NULL;
}

int create_enumerator(usb_tools_t *usb_tools, con_data_usb_t *con_data_usb)
{
    init_usb_tools_struct(usb_tools);
    init_con_data_usb_struct(con_data_usb);
    if (sd_device_enumerator_new(&usb_tools->enumerator) < 0)
        return MAJOR_ERROR;
    sd_device_enumerator_add_match_subsystem(usb_tools->enumerator, "usb", 1);
    usb_tools->device = sd_device_enumerator_get_device_first(
        usb_tools->enumerator);
    return EXIT_SUCCESS;
}
