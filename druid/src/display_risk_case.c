/**
 * @name druid
 * @version 1.0
 * @author Sacha Lemée
 * @author Fujitsu Technology Solutions
 * @file display_risk_case.c
 * @date 17 July 2025
 * @copyright Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0)
 * 
 * This file is part of the "druid" repository.
 * 
 * You can use, modify, and distribute this code under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0).
 * See the full license at: https://creativecommons.org/licenses/by-sa/4.0/deed.fr
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <systemd/sd-device.h>
#include "druid.h"

void display_couple_vendor_product_device(con_data_usb_t *con_data_usb,
    temp_data_usb_t *temp_data_usb, usb_risk_t *usb_risk)
{
    printf(
        "\e[1;37m╭───────────────────────────────────────────────── Device n°""\e[1;32m%lu\e[0m ""\e[1;37m─────────────────────────────────────────────────╮\e[0m\n"
        "│ VendorID  (\e[1;32m%s\e[0m)   │   ProductID (\e[1;32m%s\e[0m)\n"
        "│\n"
        "│ \e[1;36mFrom System\e[0m:\n"
        "│     Vendor Name (\e[1;34m%s\e[0m)   │   Product Name (\e[1;34m%s\e[0m)\n"
        "│\n"
        "│ \e[1;36mFrom Database\e[0m:\n"
        "│     Vendor Name (\e[1;34m%s\e[0m)   │   Product Name (\e[1;34m%s\e[0m)\n"
        "│\n"
        "\e[1;37m╰────────────────────────────────────────────────────────────────────────────────────────────────────────────╯\e[0m\n\n",
        usb_risk->seen_count,
        con_data_usb->vendor_id,
        con_data_usb->product_id,
        con_data_usb->vendor_name,
        con_data_usb->product_name,
        temp_data_usb->vendor_name,
        temp_data_usb->product_name);
    ++usb_risk->low;
}

void display_vendor_found_product_unknown_device(con_data_usb_t *con_data_usb,
    temp_data_usb_t *temp_data_usb, usb_risk_t *usb_risk)
{
    
    printf(
        "\e[1;37m╭───────────────────────────────────────────────── Device n°""\e[1;33m%lu\e[0m ""\e[1;37m─────────────────────────────────────────────────╮\e[0m\n"
        "│ VendorID  (\e[1;32m%s\e[0m)   │   ProductID (\e[1;31mUnknown : %s\e[0m)\n"
        "│\n"
        "│ \e[1;36mFrom System\e[0m:\n"
        "│     Vendor Name (\e[1;34m%s\e[0m)   │   Product Name (\e[1;34m%s\e[0m)\n"
        "│\n"
        "│ \e[1;36mFrom Database\e[0m:\n"
        "│     Vendor Name (\e[1;31m%s\e[0m)   │   Product Name (\e[1;31m%s\e[0m)\n"
        "│\n"
        "\e[1;37m╰────────────────────────────────────────────────────────────────────────────────────────────────────────────╯\e[0m\n\n",
        usb_risk->seen_count,
        con_data_usb->vendor_id,
        con_data_usb->product_id,
        con_data_usb->vendor_name,
        con_data_usb->product_name,
        temp_data_usb->vendor_name,
        temp_data_usb->product_name);
    ++usb_risk->medium;
}

void display_vendor_unknown_product_unknown_device(con_data_usb_t *con_data_usb,
    temp_data_usb_t *temp_data_usb, usb_risk_t *usb_risk)
{
    
    printf(
        "\e[1;37m╭───────────────────────────────────────────────── Device n°""\e[1;31m%lu\e[0m ""\e[1;37m─────────────────────────────────────────────────╮\e[0m\n"
        "│ VendorID  \e[1;31m(Unknown : %s\e[0m)   │   ProductID \e[1;31m(Unknown : %s\e[0m)\n"
        "│\n"
        "│ \e[1;36mFrom System\e[0m:\n"
        "│     Vendor Name (\e[1;31m%s\e[0m)   │   Product Name (\e[1;31m%s\e[0m)\n"
        "│\n"
        "│ \e[1;36mFrom Database\e[0m:\n"
        "│     Vendor Name (\e[1;31m%s\e[0m)   │   Product Name (\e[1;31m%s\e[0m)\n"
        "│\n"
        "\e[1;37m╰────────────────────────────────────────────────────────────────────────────────────────────────────────────╯\e[0m\n\n",
        usb_risk->seen_count,
        con_data_usb->vendor_id,
        con_data_usb->product_id,
        con_data_usb->vendor_name,
        con_data_usb->product_name,
        temp_data_usb->vendor_name,
        temp_data_usb->product_name);
    ++usb_risk->major;
}

void display_risk_table(usb_risk_t *usb_risk)
{
     printf(
        "\e[1;37m╭───────── Risk table ─────────╮\e[0m\n"
        "│ Number Low Risk    :  \e[1;32m%lu\e[0m \n"
        "│\n"
        "│ Number Medium Risk :  \e[1;33m%lu\e[0m \n"
        "│\n"
        "│ Number Major Risk  :  \e[1;31m%lu\e[0m \n"
        "\e[1;37m╰─────────────────────────────╯\e[0m\n\n",
    usb_risk->low, usb_risk->medium, usb_risk->major);

}
