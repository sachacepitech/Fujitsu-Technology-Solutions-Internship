/**
 * @name druid
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file handle_cli_info_flags.c
 * @brief Handles CLI flags related to help, format and license display
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

/**
 * @brief Handles CLI flags related to help, format and license display
 *
 * checks if specific CLI parameters are passed (help, format, license)
 * and displays the corresponding file content if matched
 * 
 * @details int handle_cli_info_flags(int ac, char **av)
 * @param ac Argument count
 * @param av Argument values
 * @return Exit code:
 *         - 0      (EXIT_SUCCESS) if a file was successfully displayed
 *         - 84     (EXIT_ERROR) if there was a critical failure while displaying
 *         - -1     (UNSEEN) if no relevant CLI flag was found
 */
int handle_cli_info_flags(int ac, char **av)
{
    size_t help_file_return_value = display_file(ac, av,
        HELP_FLAG, HELP_FLAG_OPTION, HELP_FILE);
    size_t format_file_return_value = display_file(ac, av,
        FORMAT_FLAG, FORMAT_FLAG_OPTION, FORMAT_FILE);
    size_t license_file_return_value = display_file(ac, av,
        LICENSE_FLAG, LICENSE_FLAG_OPTION, LICENSE_FILE);
    
    if (help_file_return_value == SUCCESS)
        return EXIT_SUCCESS;
    else if (help_file_return_value == EXIT_ERROR)
        return EXIT_ERROR;
    if (format_file_return_value == SUCCESS)
        return EXIT_SUCCESS;
    else if (format_file_return_value == EXIT_ERROR)
        return EXIT_ERROR;
    if (license_file_return_value == SUCCESS)
        return EXIT_SUCCESS;
    else if (license_file_return_value == EXIT_ERROR)
        return EXIT_ERROR;
    return UNSEEN;
}