/**
 * @name druid (Detection Rogue USB and Illegitimate Devices)
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file enum_windows_device_details.c
 * @brief enumerate and extract window detected device details 
 * @date 17 July 2025
 * @copyright Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0 DEED)
 * 
 * This file is part of the "druid" repository.
 * 
 * You can use, modify, and distribute this code under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0 DEED).
 * See the full license at: https://creativecommons.org/licenses/by-sa/4.0/deed.fr
 */

#include <windows.h>
#include <setupapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "druid.h"

/**
 * @brief Retrieves detailed interface information for a USB device
 *
 * Allocates memory and fills in a structure containing detailed information
 * about the device interface, including the device path
 * 
 * @details PSP_DEVICE_INTERFACE_DETAIL_DATA_A get_device_interface_detail(
 *              void* device_info_set,
 *              SP_DEVICE_INTERFACE_DATA* interface_data,
 *              SP_DEVINFO_DATA* device_info_data)
 * @param device_info_set Handle to the device information set for the current enumeration
 * @param interface_data Pointer to the SP_DEVICE_INTERFACE_DATA structure describing the interface
 * @param device_info_data Pointer to the SP_DEVINFO_DATA structure to populate with device data
 * @return Pointer to a dynamically allocated PSP_DEVICE_INTERFACE_DETAIL_DATA_A structure
 *         containing device path info, or NULL if allocation or retrieval fails
 */
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

/**
 * @brief Extracts vendor and product IDs from a hardware identifier string
 *
 * Searches the hardware ID string for "VID_" and "PID_" substrings and extracts
 * the next four hexadecimal characters following each tag into their respective buffers
 * 
 * @details void extract_vendor_and_product_ids_from_hardware_id(
 *              const char* hardware_id,
 *              char* vendor_id,
 *              char* product_id)
 * @param hardware_id Pointer to the null-terminated hardware ID string
 * @param vendor_id Pointer to a buffer (at least 5 bytes) to store the extracted vendor ID
 * @param product_id Pointer to a buffer (at least 5 bytes) to store the extracted product ID
 * @return None (void)
 */
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

/**
 * @brief Retrieves a device property as a string from the Windows registry
 *
 * Uses the SetupDiGetDeviceRegistryPropertyA function to query a property of the
 * given device and stores the result in the provided buffer
 * 
 * @details void get_device_property(
 *              void* device_info_set,
 *              SP_DEVINFO_DATA* device_info_data,
 *              unsigned long property,
 *              char* buffer,
 *              unsigned long size)
 * @param device_info_set Handle to the device information set
 * @param device_info_data Pointer to the SP_DEVINFO_DATA structure describing the device
 * @param property Identifier for the property to retrieve
 * @param buffer Pointer to a buffer to store the retrieved property value
 * @param size Size of the buffer in bytes
 * @return None (void)
 */
void get_device_property(void* device_info_set, SP_DEVINFO_DATA* device_info_data,
    unsigned long property, char* buffer, unsigned long size)
{
    memset(buffer, 0, size);
    SetupDiGetDeviceRegistryPropertyA(device_info_set, device_info_data, property,
        NULL, (unsigned char *)buffer, size, NULL);
}

