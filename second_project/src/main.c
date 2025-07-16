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
    const char *id_vendor_id = NULL;
    const char *product_id = NULL;
    const char *id_product_id = NULL;
    const char *devname = NULL;

    if (ret < 0) {
        dprintf(STDERR_FILENO, "device error in enumerator\n");
        return EXIT_FAILURE;
    }
    sd_device_enumerator_add_match_subsystem(enumerator, "usb", 2);
    device = sd_device_enumerator_get_device_first(enumerator);
    while (device != NULL) {
        sd_device_get_property_value(device, "ID_VENDOR", &vendor_id);
        sd_device_get_property_value(device, "ID_MODEL", &id_product_id);
        sd_device_get_property_value(device, "ID_VENDOR_ID", &id_vendor_id);
        sd_device_get_property_value(device, "ID_MODEL_ID", &product_id);
        sd_device_get_property_value(device, "DEVNAME", &devname);
        printf("vendor id : %s\n", vendor_id ? vendor_id : "unknown");
        printf("id product id : %s\n", id_product_id ? id_product_id : "unknown");
        printf("id vendor id : %s\n", id_vendor_id ? id_vendor_id : "unknown");
        printf("product id : %s\n", product_id ? product_id : "unknown");
        printf("device name : %s\n", devname ? devname : "unknown");
        printf("--------------------------------\n");
        device = sd_device_enumerator_get_device_next(enumerator);
    }
    sd_device_enumerator_unref(enumerator);
    return EXIT_SUCCESS;
}

int main(void)
{
    create_enumerator();
    return EXIT_SUCCESS;
}
