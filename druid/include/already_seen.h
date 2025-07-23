/**
 * @name druid
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file already_seen.h
 * @date 17 July 2025
 * @copyright Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0)
 * 
 * This file is part of the "druid" repository.
 * 
 * You can use, modify, and distribute this code under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0).
 * See the full license at: https://creativecommons.org/licenses/by-sa/4.0/deed.fr
 */

#ifndef SEEN_H
    #define SEEN_H

    #define FILE_SEPARATOR ";"

    #include <stddef.h>
    #include <systemd/sd-device.h>

typedef struct seen_device_s {
    const char *vendor_id;
    const char *product_id;
} seen_device_t;

    #define MAX_SEEN_DEVICES 128

seen_device_t seen_devices[MAX_SEEN_DEVICES];

#endif /* SEEN_H */
