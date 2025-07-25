#include <windows.h>
#include <setupapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const GUID USB_IF_GUID = { 0xA5DCBF10, 0x6530, 0x11D2, {0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED} };

/// Initialise la liste des p�riph�riques USB pr�sents.
/// Retourne un handle valide ou INVALID_HANDLE_VALUE en cas d'�chec.
HDEVINFO get_usb_device_info_list(void)
{
    return SetupDiGetClassDevsA(&USB_IF_GUID, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
}

/// Alloue et remplit les d�tails d'interface pour un p�riph�rique USB donn�.
PSP_DEVICE_INTERFACE_DETAIL_DATA_A get_device_interface_detail(HDEVINFO hDevInfo, SP_DEVICE_INTERFACE_DATA* ifData, SP_DEVINFO_DATA* devInfo)
{
    unsigned long memory_allocated = 0;
    SetupDiGetDeviceInterfaceDetailA(hDevInfo, ifData, NULL, 0, &memory_allocated, NULL);

    PSP_DEVICE_INTERFACE_DETAIL_DATA_A pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA_A)malloc(memory_allocated);
    if (!pDetail) {
    }
    pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);
    devInfo->cbSize = sizeof(SP_DEVINFO_DATA);
    if (!SetupDiGetDeviceInterfaceDetailA(hDevInfo, ifData, pDetail, memory_allocated, NULL, devInfo)) {
        free(pDetail);
        return NULL;
    }
    return pDetail;
}

/// Extrait les VID et PID � partir du champ Hardware ID.
void extract_vendor_and_product_ids(const char* hardware_id, char* vendor_id, char* product_id)
{
    char* vendor = strstr(hardware_id, "VID_");
    char* product = strstr(hardware_id, "PID_");

    memset(vendor_id, '?', 4);
    vendor_id[4] = '\0';
    memset(product_id, '?', 4);
    product_id[4] = '\0';
    if (vendor)
        strncpy(vendor_id, vendor + 4, 4);
    if (product)
        strncpy(product_id, product + 4, 4);
}

/// R�cup�re une propri�t� de registre du p�riph�rique.
void get_device_property(HDEVINFO hDevInfo, SP_DEVINFO_DATA* devInfo, unsigned long property, char* buffer, unsigned long size)
{
    memset(buffer, 0, size);
    SetupDiGetDeviceRegistryPropertyA(hDevInfo, devInfo, property, NULL, (BYTE*)buffer, size, NULL);
}

/// Affiche les informations du p�riph�rique USB.
void display_device_info(HDEVINFO hDevInfo, SP_DEVINFO_DATA* devInfo)
{
    char hardware_id[512];
    char vendor_id[5], product_id[5];
    char vendor_name[256];
    char product_name[256];

    get_device_property(hDevInfo, devInfo, SPDRP_HARDWAREID, hardware_id, sizeof(hardware_id));
    extract_vendor_and_product_ids(hardware_id, vendor_id, product_id);
    get_device_property(hDevInfo, devInfo, SPDRP_MFG, vendor_name, sizeof(vendor_name));
    get_device_property(hDevInfo, devInfo, SPDRP_DEVICEDESC, product_name, sizeof(product_name));
    printf("USB Device: VID=%s PID=%s\n  vendor_name: %s\n  product_name: %s\n\n", vendor_id, product_id, vendor_name, product_name);
}

/// Parcourt et affiche tous les p�riph�riques USB connect�s.
void enumerate_usb_devices(void)
{
    HDEVINFO hDevInfo = get_usb_device_info_list();
    SP_DEVICE_INTERFACE_DATA ifData = { 0 };
    SP_DEVINFO_DATA devInfo = { 0 };
    PSP_DEVICE_INTERFACE_DETAIL_DATA_A pDetail = { 0 };

    if (hDevInfo == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Erreur : SetupDiGetClassDevsA.\n");
        return;
    }
    ifData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    for (unsigned long i = 0; SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &USB_IF_GUID, i, &ifData); ++i) {
        pDetail = get_device_interface_detail(hDevInfo, &ifData, &devInfo);
        if (pDetail == NULL)
            continue;
        display_device_info(hDevInfo, &devInfo);
        free(pDetail);
    }
    SetupDiDestroyDeviceInfoList(hDevInfo);
}

int main(void) {
    enumerate_usb_devices();
    return 0;
}
