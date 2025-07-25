/**
 * @name druid (Detection Rogue USB and Illegitimate Devices)
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file init_usb_enumerator.c
 * @brief initializes the USB enumerator
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
#include <stddef.h>
#include "druid.h"

/**
 * @brief Initializes the USB enumerator
 *
 * Sets up internal USB tool structures and prepares the Windows
 * device enumeration for targeting the USB subsystem. Also initializes
 * the USB device info structure.
 * 
 * @details int init_usb_enumerator(usb_tools_t *usb_tools, usb_device_info_t *usb_device_info)
 * @param usb_tools Pointer to the usb_tools_t structure used for enumeration
 * @param usb_device_info Pointer to the usb_device_info_t structure to be initialized
 * @return Exit code:
 *         - 0      (EXIT_SUCCESS) on succes
 *         - 84     (EXIT_ERROR) on critical failure
 */
int init_usb_enumerator(usb_tools_t *usb_tools, usb_device_info_t *usb_device_info)
{
    init_struct_usb_device_info(usb_device_info);

    usb_tools->device_info_set = SetupDiGetClassDevsA(
        &USB_IF_GUID, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (usb_tools->device_info_set == INVALID_HANDLE_VALUE)
        return EXIT_ERROR;
    usb_tools->index = 0;
    return EXIT_SUCCESS;
}
