/**
 * @name druid (Detection Rogue USB and Illegitimate Devices)
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file main.c
 * @brief main function, entry point
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
 * @brief Main function
 *
 * initializes structures, enumerates connected usb devices
 * and handles cases where parameters are set as cli (user input)
 * 
 * @details int main(int ac, char **av)
 * @param ac Argument count
 * @param av Argument values
 * @return Exit code:
 *         - 0      (EXIT_SUCCESS) on success
 *         - 84     (EXIT_ERROR) on critical failure
 */
int main(int ac, char **av)
{
    usb_device_info_t usb_device_info = {0};
    usb_tools_t usb_tools = {0};
    usb_db_entry_t usb_db_entry = {0};
    cli_args_t cli_args = {ac, av};
    int cli_flags_result = handle_cli_info_flags(ac, av);

    if (cli_flags_result == EXIT_SUCCESS)
        return EXIT_SUCCESS;
    else if (cli_flags_result == EXIT_ERROR)
        return EXIT_ERROR;
    if (init_usb_enumerator(&usb_tools, &usb_device_info) == EXIT_ERROR)
        return EXIT_ERROR;
    if (scan_connected_usb_and_check_risks(&usb_tools, &usb_device_info, &usb_db_entry, &cli_args) == EXIT_ERROR) {
        sd_device_enumerator_unref(usb_tools.enumerator);
        return EXIT_ERROR;
    }
    sd_device_enumerator_unref(usb_tools.enumerator);
    return EXIT_SUCCESS;
}
