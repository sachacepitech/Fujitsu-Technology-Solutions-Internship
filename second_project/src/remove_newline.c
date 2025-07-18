/*
** FUJITSU PROJECT, 2025
** second_project
** Version:
** 1.0
** File description:
** remove_newline.c
** Author:
** Sacha Lemée, @sacha-lemee on Linkedin
** Junior Cybersecurity Consultant
** Created:
** ???
*/

#include <stdio.h>
#include <string.h>

#include "dependencies_project.h"

void remove_newline(char *str)
{
    size_t len = strlen(str);

    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}
