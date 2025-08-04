/**
 * @name druid (Detection Rogue USB and Illegitimate Devices)
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file display_file.c
 * @brief function use to display file(s)
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
 * @brief Displays the contents of a file if the correct flag is passed via CLI
 *
 * Opens the specified file and prints its contents line by line to stdout
 * if the provided arguments contain the matching flag or optional flag
 * 
 * @details int display_file(
 *             int ac,
 *             char **av,
 *             const char *flag,
 *             const char *optional_flag,
 *             const char *path_file)
 * @param ac Argument count from CLI
 * @param av Argument vector from CLI
 * @param flag Primary flag to trigger file display (e.g., "-h")
 * @param optional_flag Alternate flag to trigger file display (e.g., "--help")
 * @param path_file Path to the file to be displayed
 * @return Exit code:
 *         - 0      (SUCCESS) if file was displayed successfully
 *         - 1      if conditions weren't met (flag missing or incorrect argc)
 *         - 84     (EXIT_ERROR) if the file failed to open
 */
int display_file(int ac, char **av, const char *flag,
    const char *optional_flag, const char *path_file)
{
    FILE *fd = 0;
    char *line = NULL;
    size_t n = 0;
    int return_value = 1;

    if (ac == 2 && (strcmp(av[1], flag) == SUCCESS
        || strcmp(av[1], optional_flag) == SUCCESS)) {
        fd = fopen(path_file, READ_MODE);
        if (fd == NULL)
            return EXIT_ERROR;
        return_value = 0;
        while (getline(&line, &n, fd) != EOF)
            printf("%s", line);
        free(line);
        fclose(fd);
    }
    return return_value;
}
