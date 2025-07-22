/**
 * @name second project
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file flags_files.c
 * @date 17 July 2025
 * @copyright Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0)
 * 
 * This file is part of the "second project" repository.
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

#include "dependencies_project.h"

int flags_files(int ac, char **av)
{
    size_t help_file_return_value = display_file(ac, av, "-h", "--help", HELP_FILE);
    size_t format_file_return_value = display_file(ac, av, "-f", "--format", FORMAT_FILE);
    size_t license_file_return_value = display_file(ac, av, "-l", "--license", LICENSE_FILE);
    
    if (help_file_return_value == SUCCESS)
        return EXIT_SUCCESS;
    else if (help_file_return_value == MAJOR_ERROR)
        return MAJOR_ERROR;
    if (format_file_return_value == SUCCESS)
        return EXIT_SUCCESS;
    else if (format_file_return_value == MAJOR_ERROR)
        return MAJOR_ERROR;
    if (license_file_return_value == SUCCESS)
        return EXIT_SUCCESS;
    else if (license_file_return_value == MAJOR_ERROR)
        return MAJOR_ERROR;
    return EXIT_FAILURE;
}