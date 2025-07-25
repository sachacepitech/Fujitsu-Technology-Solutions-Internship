/**
 * @name druid (Detection Rogue USB and Illegitimate Devices)
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file druid.h
 * @brief definitions and prototypes for usb device management and associated database handling
 * @date 17 July 2025
 * @copyright Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0 DEED)
 * 
 * This file is part of the "druid" repository.
 * 
 * You can use, modify, and distribute this code under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0 DEED).
 * See the full license at: https://creativecommons.org/licenses/by-sa/4.0/deed.fr
 */

#ifndef DRUID_H
    #define DRUID_H
    
    /* standard return codes */
    #define SUCCESS 0
    #define UNSEEN -1
    #define EXIT_ERROR 84

    /* device type for systemd filtering */
    #define SEARCH_DEVICE_TYPE "usb"

    /* file format and read mode */
    #define FILE_SEPARATOR ";"
    #define FILE_TYPE_SEPARATOR "."
    #define FILE_TYPE "csv"
    #define FILE_TYPE_PLUS_SEPARATOR ".csv"
    #define READ_MODE "r"
    
    /* default database file path */
    #define DATA_FILE_PATH "data-files/vendor_id_product_id_and_name.csv"
    
    /* paths to cli info/help files */
    #define HELP_FILE "src/INFO_FILE/HELP"
    #define FORMAT_FILE "src/INFO_FILE/FORMAT"
    #define LICENSE_FILE "src/INFO_FILE/LICENSE"
    
    /* systemd device property keys */
    #define VENDOR_ID "ID_VENDOR_ID"
    #define VENDOR_NAME "ID_VENDOR"
    #define PRODUCT_ID "ID_MODEL_ID"
    #define PRODUCT_NAME "ID_MODEL"

    /* cli flag macros */
    #define HELP_FLAG "-h"
    #define FORMAT_FLAG "-f"
    #define LICENSE_FLAG "-l"
    #define UPDATE_FLAG "-u"
    #define HELP_FLAG_OPTION "--help"
    #define FORMAT_FLAG_OPTION "--format"
    #define LICENSE_FLAG_OPTION "--license"
    #define UPDATE_FLAG_OPTION "--update"

    /* default messages */
    #define UNKNOWN_DEVICE_MESSAGE "Unknown"
    #define NONE_CSV_FILE_MESSAGE "Error: Make sure it is a csv file.\n"

    /* cli argument position macros */
    #define cli_args_FLAG 1
    #define cli_args_FILE 2
    #define BIN_FLAG_FILE 3

    #include <stddef.h>
    #include <windows.h>
    #include <setupapi.h>

/**
 * @brief represents a single entry in the usb device database
*/
typedef struct usb_db_entry_s {
    char *vendor_id;
    char *vendor_name;
    char *product_id;
    char *product_name;
} usb_db_entry_t;

/**
 * @brief stores information about a connected usb device
*/
typedef struct data_usb_con_s {
    char *vendor_id;
    char *vendor_name;
    char *product_id;
    char *product_name;
    char *path_usb;
} usb_device_info_t;

/**
 * @brief holds systemd usb device enumeration tools
*/
typedef struct usb_tools_s {
    HDEVINFO device_info_set;
    SP_DEVICE_INTERFACE_DATA interface_data;
    unsigned long index;
} usb_tools_t;

    /* initial allocation size for database entries */
    #define DEFAULT_SIZE 10
    #define INCREASED_SIZE 2

/**
 * @brief represents the entire usb device database
*/
typedef struct usb_db_s {
    usb_db_entry_t *entries;
    long unsigned int count;
} usb_db_t;

/**
 * @brief stores statistics about usb risk levels
*/
typedef struct usb_risk_stats_s {
    long unsigned int low;
    long unsigned int medium;
    long unsigned int major;
    long unsigned int seen_count;
} usb_risk_stats_stats_t;

/**
 * @brief holds cli arguments for structured parsing
*/
typedef struct cli_args_s {
    int ac;
    char **av;
} cli_args_t;

/* init all */
void init_struct_usb_device_info(usb_device_info_t *usb_device_info);
int init_struct_usb_db(usb_db_t *usb_db, long unsigned int allocated_capacity);
void init_struct_usb_db_entry(usb_db_entry_t *usb_db_entry);
void init_struct_unknown_usb_db_entry(usb_db_entry_t *unknown);
int init_usb_enumerator(usb_tools_t *usb_tools, usb_device_info_t *usb_device_info);

/* fill database struct */
int load_usb_db_from_file(usb_db_t *usb_db, usb_db_entry_t *usb_db_entry, cli_args_t *cli_args);

/* free all */
void free_unknown_usb_db_entry(usb_db_entry_t *unknown);
void free_usb_db(usb_db_t *usb_db);

/* display risk case */
void display_known_usb_device(usb_device_info_t *usb_device_info,
    usb_db_entry_t *usb_db_entry, usb_risk_stats_stats_t *usb_risk_stats);
void display_partially_known_usb_device(usb_device_info_t *usb_device_info,
    usb_db_entry_t *usb_db_entry, usb_risk_stats_stats_t *usb_risk_stats);
void display_unknown_usb_device(usb_device_info_t *usb_device_info,
    usb_db_entry_t *usb_db_entry, usb_risk_stats_stats_t *usb_risk_stats);
void display_risk_table(usb_risk_stats_stats_t *usb_risk_stats);

/* option */
int handle_cli_info_flags(int ac, char **av);
int display_file(int ac, char **av, const char *flag,
    const char *optional_flag, const char *path_file);

/* core comparison function */
int scan_connected_usb_and_check_risks(usb_tools_t *usb_tools, usb_device_info_t *usb_device_info,
    usb_db_entry_t *usb_db_entry, cli_args_t *cli_args);
void extract_vendor_and_product_ids_from_hardware_id(const char* hardware_id,
    char* vendor_id, char* product_id);
void get_device_property(void* device_info_set, SP_DEVINFO_DATA* device_info_data,
    unsigned long property, char* buffer, unsigned long size);
PSP_DEVICE_INTERFACE_DETAIL_DATA_A get_device_interface_detail(void* device_info_set,
    SP_DEVICE_INTERFACE_DATA* interface_data, SP_DEVINFO_DATA* device_info_data);

// This is a static definition of a class identifier (GUID) for USB device interfaces in Windows.
// It allows you to ask Windows “Give me all the devices that expose this interface” via the SetupDiGetClassDevs API.
static const GUID USB_IF_GUID = { 0xA5DCBF10, 0x6530, 0x11D2, {0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED} };

#endif /* DRUID_H */
