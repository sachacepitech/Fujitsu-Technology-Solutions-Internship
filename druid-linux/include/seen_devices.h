/**
 * @name druid (Detection Rogue USB and Illegitimate Devices)
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file seen_devices.h
 * @brief defines structures and storage for tracking already seen devices
 * @date 17 July 2025
 * @copyright Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0 DEED)
 * 
 * This file is part of the "druid" repository.
 * 
 * You can use, modify, and distribute this code under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0 DEED).
 * See the full license at: https://creativecommons.org/licenses/by-sa/4.0/deed.fr
 */

#ifndef SEEN_DEVICES_H
    #define SEEN_DEVICES_H
    #include <stddef.h>
    #include <systemd/sd-device.h>

/**
 * @brief represents a minimal usb device with vendor and product ID's
*/
typedef struct seen_device_s {
    const char *vendor_id;
    const char *product_id;
} seen_device_t;

    /* mmaximum number of usb devices to track as "seen" */
    #define MAX_SEEN_DEVICES 128

/* global array to track already processed usb devices */
seen_device_t seen_devices[MAX_SEEN_DEVICES];

#endif /* SEEN_DEVICES_H */
