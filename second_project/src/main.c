#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>

#include <systemd/sd-device.h>

#include "dependencies_project.h"

int main(void)
{
    if (create_enumerator() == EXIT_FAILURE)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
