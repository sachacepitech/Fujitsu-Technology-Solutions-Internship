/**
 * @name druid
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file create_enumerator.c
 * @date 17 July 2025
 * @copyright Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0)
 * 
 * This file is part of the "druid" repository.
 * 
 * You can use, modify, and distribute this code under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0).
 * See the full license at: https://creativecommons.org/licenses/by-sa/4.0/deed.fr
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <systemd/sd-device.h>
#include "druid.h"

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
