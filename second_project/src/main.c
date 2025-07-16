#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>

#include <systemd/sd-device.h>

#include "dependencies_project.h"

// const char *action = NULL;
// const char *vendor_id = NULL;
// const char *product_id = NULL;
// 
// sd_device_get_property_value(device, "VENDOR_ID", &vendor_id);
// sd_device_get_property_value(device, "ID_MODEL_ID", &product_id);

int create_enumerator(void)
{
    sd_device *device = NULL;
    sd_device_enumerator *enumerator = NULL;
    int ret = sd_device_enumerator_new(&enumerator);
    const char *vendor_id = NULL;
    const char *product_id = NULL;

    if (ret < 0) {
        dprintf(STDERR_FILENO, "device error in enumerator\n");
        return EXIT_FAILURE;
    }
    sd_device_enumerator_add_match_subsystem(enumerator, "usb", 1);
    device = sd_device_enumerator_get_device_first(enumerator);
    while (device != NULL) {
        sd_device_get_property_value(device, "VENDOR_ID", &vendor_id);
        sd_device_get_property_value(device, "ID_MODEL_ID", &product_id);
        device = sd_device_enumerator_get_device_next(enumerator);
    }
    sd_device_enumerator_unref(enumerator);
    printf("test\n");
    return EXIT_SUCCESS;
}

int main(void)
{
    create_enumerator();
    return EXIT_SUCCESS;
}
