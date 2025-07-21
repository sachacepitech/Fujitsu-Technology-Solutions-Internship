/**
 * @name second project
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file main.c
 * @date 17 July 2025
 * @copyright have to fill
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>

#include <systemd/sd-device.h>

#include "dependencies_project.h"

/**
 * le
*/
int main(void)
{
    con_data_usb_t con_data_usb = {0};
    usb_tools_t usb_tools = {0};
    temp_data_usb_t temp_data_usb = {0};

    if (create_enumerator(&usb_tools, &con_data_usb) == MAJOR_ERROR)
        return MAJOR_ERROR;
    if (comparator(&usb_tools, &con_data_usb, &temp_data_usb) == MAJOR_ERROR)
        return MAJOR_ERROR;
    sd_device_enumerator_unref(usb_tools.enumerator);
    return EXIT_SUCCESS;
}
