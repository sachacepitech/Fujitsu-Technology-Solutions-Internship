// dependencies lib
#include "dependencies_project.h"

// lib
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>

// usb lib
#include <libudev.h>

int main(void)
{
    struct udev *udev;

    printf("Welcome to Usb Detection project\n");
    udev = udev_new();
    if (!udev) {
        printf("Failed to create udev context\n");
        return EXIT_FAIL;
    }
    printf("Udev context created successfully\n");
    udev_unref(udev);
    return EXIT_SUCCESS;
}
