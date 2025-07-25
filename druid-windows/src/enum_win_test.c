#include <windows.h>
#include <setupapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const GUID USB_IF_GUID = { 0xA5DCBF10, 0x6530, 0x11D2, {0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED} };

/// Initialise la liste des périphériques USB présents.
/// Retourne un handle valide ou INVALID_HANDLE_VALUE en cas d'échec.
HDEVINFO getUsbDeviceInfoList(void)
{
    return SetupDiGetClassDevsA(&USB_IF_GUID, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
}

/// Alloue et remplit les détails d'interface pour un périphérique USB donné.
PSP_DEVICE_INTERFACE_DETAIL_DATA_A get_device_interface_detail(HDEVINFO hDevInfo, SP_DEVICE_INTERFACE_DATA* ifData, SP_DEVINFO_DATA* devInfo)
{
    DWORD requiredSize = 0;
    SetupDiGetDeviceInterfaceDetailA(hDevInfo, ifData, NULL, 0, &requiredSize, NULL);

    PSP_DEVICE_INTERFACE_DETAIL_DATA_A pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA_A)malloc(requiredSize);
    if (!pDetail) {
        fprintf(stderr, "Erreur : allocation mémoire échouée.\n");
        return NULL;
    }
    pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);
    devInfo->cbSize = sizeof(SP_DEVINFO_DATA);

    if (!SetupDiGetDeviceInterfaceDetailA(hDevInfo, ifData, pDetail, requiredSize, NULL, devInfo)) {
        free(pDetail);
        return NULL;
    }

    return pDetail;
}

/// Extrait les VID et PID à partir du champ Hardware ID.
void extractVidPid(const char* hardware_id, char* vendor_id, char* product_id)
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
        strncpy(product_id,product + 4, 4);
}

/// Récupère une propriété de registre du périphérique.
void getDeviceProperty(HDEVINFO hDevInfo, SP_DEVINFO_DATA* devInfo, DWORD property, char* buffer, DWORD size)
{
    memset(buffer, 0, size);
    SetupDiGetDeviceRegistryPropertyA(hDevInfo, devInfo, property, NULL, (BYTE*)buffer, size, NULL);
}

/// Affiche les informations du périphérique USB.
void printDeviceInfo(HDEVINFO hDevInfo, SP_DEVINFO_DATA* devInfo)
{
    char hardware_id[512];
    char vendor_id[5], product_id[5];
    char manufacturer[256];
    char description[256];

    getDeviceProperty(hDevInfo, devInfo, SPDRP_HARDWAREID, hardware_id, sizeof(hardware_id));
    extractVidPid(hardware_id, vendor_id, product_id);
    getDeviceProperty(hDevInfo, devInfo, SPDRP_MFG, manufacturer, sizeof(manufacturer));
    getDeviceProperty(hDevInfo, devInfo, SPDRP_DEVICEDESC, description, sizeof(description));
    printf("USB Device: VID=%s PID=%s\n  Manufacturer: %s\n  Description: %s\n\n", vendor_id, product_id, manufacturer, description);
}

/// Parcourt et affiche tous les périphériques USB connectés.
void enumerate_usb_devices(void)
{
    HDEVINFO hDevInfo = getUsbDeviceInfoList();
    if (hDevInfo == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Erreur : SetupDiGetClassDevsA a échoué.\n");
        return;
    }
    SP_DEVICE_INTERFACE_DATA ifData;
    ifData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    for (DWORD idx = 0; SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &USB_IF_GUID, idx, &ifData); idx++) {
        SP_DEVINFO_DATA devInfo;
        PSP_DEVICE_INTERFACE_DETAIL_DATA_A pDetail = get_device_interface_detail(hDevInfo, &ifData, &devInfo);
        if (!pDetail)
            continue;
        printDeviceInfo(hDevInfo, &devInfo);
        free(pDetail);
    }
    SetupDiDestroyDeviceInfoList(hDevInfo);
}

int main(void) {
    enumerate_usb_devices();
    return 0;
}
