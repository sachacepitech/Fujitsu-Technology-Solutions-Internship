/*
** FUJITSU PROJECT, 2025
** second_project
** Version:
** 1.0
** File description:
** main.c
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

int main(void)
{
    con_data_usb_t con_data_usb = {0};
    usb_tools_t usb_tools = {0};

    if (create_enumerator(&usb_tools, &con_data_usb) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if (comparator(&usb_tools, &con_data_usb) == EXIT_FAILURE)
        return EXIT_FAILURE;
    sd_device_enumerator_unref(usb_tools.enumerator);
    return EXIT_SUCCESS;
}
