#include "dependencies_project.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>

#include <libudev.h>

int create_monitor(struct udev *udev)
{
    struct udev_monitor *monitor = udev_monitor_new_from_netlink(udev, "udev");
    struct udev_device *device = {0};
    const char *vendor_id = NULL;
    const char *product_id = NULL;

    if (monitor == NULL) {
        printf("failed to create udev monitor\n");
        return EXIT_FAILURE;
    }
    udev_monitor_filter_add_match_subsystem_devtype(monitor, "usb", NULL);
    udev_monitor_enable_receiving(monitor);

    device = udev_monitor_receive_device(monitor);
    if (device == NULL) {
        printf("no event found");
        udev_device_unref(monitor);
        return EXIT_FAILURE;
    }
    vendor_id = udev_device_get_property_value(device, "ID_VENDOR_ID");
    product_id = udev_device_get_property_value(device, "ID_MODEL_ID");
    
    printf("vendor_id : %s\n", vendor_id);
    printf("product_id : %s\n", product_id);

    udev_device_unref(device);
    udev_device_unref(monitor);
    return EXIT_SUCCESS;
}

int create_context(void)
{
    struct udev *udev = {0};

    printf("welcome to usb detection project\n");
    udev = udev_new();
    if (udev == NULL) {
        printf("failed to create udev context\n");
        return EXIT_FAILURE;
    }
    create_monitor(udev);
    udev_unref(udev);
    return EXIT_SUCCESS;
}

int main(void)
{
    create_context();
    return EXIT_SUCCESS;
}
