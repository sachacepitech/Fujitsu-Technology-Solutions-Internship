/**
 * @name second project
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file already_seen.h
 * @date 17 July 2025
 * @copyright have to fill
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

seen_device_t seen[MAX_SEEN_DEVICES];

#endif /* SEEN_H */
