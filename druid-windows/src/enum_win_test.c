#include <windows.h>
#include <setupapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "druid.h"

PSP_DEVICE_INTERFACE_DETAIL_DATA_A get_device_interface_detail(void* device_info_set,
    SP_DEVICE_INTERFACE_DATA* interface_data, SP_DEVINFO_DATA* device_info_data)
{
    unsigned long memory_allocated = 0;
    PSP_DEVICE_INTERFACE_DETAIL_DATA_A interface_detail = NULL;

    SetupDiGetDeviceInterfaceDetailA(device_info_set, interface_data, NULL, 0, &memory_allocated, NULL);
    interface_detail = (PSP_DEVICE_INTERFACE_DETAIL_DATA_A)malloc(memory_allocated);
    if (interface_detail == NULL)
        return NULL;
    interface_detail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);
    device_info_data->cbSize = sizeof(SP_DEVINFO_DATA);
    if (SetupDiGetDeviceInterfaceDetailA(device_info_set, interface_data, interface_detail, memory_allocated, NULL, device_info_data) == false) {
        free(interface_detail);
        return NULL;
    }
    return interface_detail;
}

void extract_vendor_and_product_ids_from_hardware_id(const char* hardware_id,
    char* vendor_id, char* product_id)
{
    char* vendor = strstr(hardware_id, "VID_");
    char* product = strstr(hardware_id, "PID_");

    memset(vendor_id, '?', 4);
    vendor_id[4] = '\0';
    memset(product_id, '?', 4);
    product_id[4] = '\0';
    strncpy(vendor_id, vendor + 4, 4);
    strncpy(product_id, product + 4, 4);
}

void get_device_property(void* device_info_set, SP_DEVINFO_DATA* device_info_data,
    unsigned long property, char* buffer, unsigned long size)
{
    memset(buffer, 0, size);
    SetupDiGetDeviceRegistryPropertyA(device_info_set, device_info_data, property, NULL, (unsigned char *)buffer, size, NULL);
}

