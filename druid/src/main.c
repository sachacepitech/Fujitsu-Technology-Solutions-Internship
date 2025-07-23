/**
 * @name DRUID
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file main.c
 * @date 17 July 2025
 * @copyright Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0)
 * 
 * This file is part of the "DRUID" repository.
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

int main(int ac, char **av)
{
    con_data_usb_t con_data_usb = {0};
    usb_tools_t usb_tools = {0};
    temp_data_usb_t temp_data_usb = {0};
    param_t param = {ac, av};
    int return_value_flags_file = flags_files(ac, av);

    if (return_value_flags_file == SUCCESS)
        return SUCCESS;
    else if (return_value_flags_file == MAJOR_ERROR)
        return MAJOR_ERROR;
    if (create_enumerator(&usb_tools, &con_data_usb) == MAJOR_ERROR)
        return MAJOR_ERROR;
    if (comparator(&usb_tools, &con_data_usb, &temp_data_usb, &param) == MAJOR_ERROR) {
        sd_device_enumerator_unref(usb_tools.enumerator);
        return MAJOR_ERROR;
    }
    sd_device_enumerator_unref(usb_tools.enumerator);
    return EXIT_SUCCESS;
}
