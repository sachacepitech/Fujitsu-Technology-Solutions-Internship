#include <windows.h>
#include <setupapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// This is a static definition of a class identifier (GUID) for USB device interfaces in Windows.
static const GUID USB_IF_GUID = { 0xA5DCBF10, 0x6530, 0x11D2, {0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED} };
// It allows you to ask Windows “Give me all the devices that expose this interface” via the SetupDiGetClassDevs API.

void* get_usb_device_info_list(void)
{
    return SetupDiGetClassDevsA(&USB_IF_GUID, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	// 'A' at the end of SetupDiGetClassDevsA like function, correspond to the ANSI version of the function.
}

PSP_DEVICE_INTERFACE_DETAIL_DATA_A get_device_interface_detail(void* device_info_set,
    SP_DEVICE_INTERFACE_DATA* interface_data, SP_DEVINFO_DATA* device_info_data)
{
    unsigned long memory_allocated = 0;
    PSP_DEVICE_INTERFACE_DETAIL_DATA_A interface_detail = NULL;

    SetupDiGetDeviceInterfaceDetailA(device_info_set, interface_data, NULL, 0, &memory_allocated, NULL);
    interface_detail = (PSP_DEVICE_INTERFACE_DETAIL_DATA_A)malloc(memory_allocated);
    if (interface_detail == NULL) {
        return NULL;
    }
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

void display_device_info(void* device_info_set, SP_DEVINFO_DATA* device_info_data)
{
    char hardware_id[512];
    char vendor_id[5];
    char product_id[5];
    char vendor_name[256];
    char product_name[256];

    get_device_property(device_info_set, device_info_data, SPDRP_HARDWAREID, hardware_id, sizeof(hardware_id));
    extract_vendor_and_product_ids_from_hardware_id(hardware_id, vendor_id, product_id);
    get_device_property(device_info_set, device_info_data, SPDRP_MFG, vendor_name, sizeof(vendor_name));
    get_device_property(device_info_set, device_info_data, SPDRP_DEVICEDESC, product_name, sizeof(product_name));
    printf("USB Device:\n"
        "\tVendorID (%s)   |   ProductID (%s)\n"
        "\tVendor Name (%s)  |   Product Name (%s)\n\n"
        "---------------------------------------------------\n",
        vendor_id, product_id, vendor_name, product_name);
}

void enumerate_usb_devices(void)
{
    void* device_info_set = get_usb_device_info_list();
    SP_DEVICE_INTERFACE_DATA interface_data = { 0 };
    SP_DEVINFO_DATA device_info_data = { 0 };
    PSP_DEVICE_INTERFACE_DETAIL_DATA_A interface_detail = { 0 };

    if (device_info_set == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Erreur : SetupDiGetClassDevsA.\n");
        return;
    }
    interface_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    for (unsigned long i = 0; SetupDiEnumDeviceInterfaces(device_info_set, NULL, &USB_IF_GUID, i, &interface_data); ++i) {
        interface_detail = get_device_interface_detail(device_info_set, &interface_data, &device_info_data);
        if (interface_detail == NULL)
            continue;
        display_device_info(device_info_set, &device_info_data);
        free(interface_detail);
    }
    SetupDiDestroyDeviceInfoList(device_info_set);
}

int main(void) {
    enumerate_usb_devices();
    return 0;
}
