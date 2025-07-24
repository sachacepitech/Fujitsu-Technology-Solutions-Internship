#include <windows.h>
#include <setupapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const GUID USB_IF_GUID = { 0xA5DCBF10, 0x6530, 0x11D2, {0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED} };

int main(void)
{
    HDEVINFO hDevInfo;
    SP_DEVICE_INTERFACE_DATA ifData;
    DWORD idx;
    DWORD required;
    PSP_DEVICE_INTERFACE_DETAIL_DATA_A pDetail;
    SP_DEVINFO_DATA devInfo;
    char hwID[512];
    char mf[256];
    char desc[256];
    char vid[5];
    char pid[5];
    char* v;
    char* p;

    hDevInfo = SetupDiGetClassDevsA(&USB_IF_GUID, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (hDevInfo == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Erreur : SetupDiGetClassDevsA a échoué.\n");
        return 1;
    }
    ifData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    for (idx = 0; SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &USB_IF_GUID, idx, &ifData); idx++) {
        required = 0;
        SetupDiGetDeviceInterfaceDetailA(hDevInfo, &ifData, NULL, 0, &required, NULL);
        pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA_A)malloc(required);
        if (pDetail == NULL) {
            fprintf(stderr, "Erreur : allocation mémoire échouée.\n");
            break;
        }
        pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);
        devInfo.cbSize = sizeof(SP_DEVINFO_DATA);
        if (SetupDiGetDeviceInterfaceDetailA(hDevInfo, &ifData, pDetail, required, NULL, &devInfo)) {
            memset(hwID, 0, sizeof(hwID));
            memset(mf, 0, sizeof(mf));
            memset(desc, 0, sizeof(desc));
            memset(vid, '?', sizeof(vid) - 1);
            vid[4] = '\0';
            memset(pid, '?', sizeof(pid) - 1);
            pid[4] = '\0';
            if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfo, SPDRP_HARDWAREID, NULL, (BYTE*)hwID, sizeof(hwID), NULL)) {
                v = strstr(hwID, "VID_");
                p = strstr(hwID, "PID_");
                if (v != NULL) {
                    strncpy(vid, v + 4, 4);
                    vid[4] = '\0';
                }
                if (p != NULL) {
                    strncpy(pid, p + 4, 4);
                    pid[4] = '\0';
                }
            }
            SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfo, SPDRP_MFG, NULL, (BYTE*)mf, sizeof(mf), NULL);
            SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfo, SPDRP_DEVICEDESC, NULL, (BYTE*)desc, sizeof(desc), NULL);
            printf("USB Device: VID=%s PID=%s\n  Manufacturer: %s\n  Description: %s\n\n", vid, pid, mf, desc);
        }
        free(pDetail);
    }
    SetupDiDestroyDeviceInfoList(hDevInfo);
    return 0;
}
