/**
 * @name druid
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file init_usb_enumerator.c
 * @brief Initializes the USB enumerator
 * @date 17 July 2025
 * @copyright Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0 DEED)
 * 
 * This file is part of the "druid" repository.
 * 
 * You can use, modify, and distribute this code under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0 DEED).
 * See the full license at: https://creativecommons.org/licenses/by-sa/4.0/deed.fr
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <systemd/sd-device.h>
#include "druid.h"

/**
 * @brief Initializes the USB enumerator
 *
 * sets up internal USB tool structures, initializes the USB device info,
 * and prepares the systemd enumerator to target the USB subsystem
 * 
 * @details int init_usb_enumerator(usb_tools_t *usb_tools, usb_device_info_t *usb_device_info)
 * @param usb_tools Pointer to the usb_tools_t structure used for enumeration
 * @param usb_device_info Pointer to the usb_device_info_t structure to be initialized
 * @return Exit code:
 *         - 0      (EXIT_SUCCESS) on successful initialization
 *         - 84     (EXIT_ERROR) if the enumerator could not be created
 */
int init_usb_enumerator(usb_tools_t *usb_tools, usb_device_info_t *usb_device_info)
{
    init_struct_usb_tools(usb_tools);
    init_struct_usb_device_info(usb_device_info);
    if (sd_device_enumerator_new(&usb_tools->enumerator) < 0)
        return EXIT_ERROR;
    sd_device_enumerator_add_match_subsystem(usb_tools->enumerator, SEARCH_DEVICE_TYPE, 1);
    usb_tools->device = sd_device_enumerator_get_device_first(
        usb_tools->enumerator);
    return EXIT_SUCCESS;
}
