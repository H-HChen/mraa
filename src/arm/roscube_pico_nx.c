 /*
 * Author: Chester Tseng <chester.tseng@adlinktech.com>

 * Copyright (c) 2020 ADLINK Technology Inc. Advanced Robotic Platform Group
 * SPDX-License-Identifier: MIT

*/

#include <stdio.h>
#include "arm/roscube_pico_nx.h"

#define MAX_DEVPATH_LENGTH 64
#define MRAA_ROSCUBE_PICO_NX_BOARD_NAME "ROScube Pico NX"
#define BOARD_UART_NAME "/dev/ttyTHS1"

mraa_result_t
mraa_roscube_pico_nx_uart_init_pre(int index) {

    // There's only 1 UART port in ROScube Pico NX (/dev/ttyTHS1)
    if (!mraa_file_exist(BOARD_UART_NAME)) {
        syslog(LOG_ERR, "uart: Device not initialized");
    } else {
        plat->uart_dev[index].device_path = BOARD_UART_NAME;
        return MRAA_SUCCESS;
    }

    return MRAA_ERROR_INVALID_PARAMETER;
}

mraa_result_t 
mraa_roscube_pico_nx_i2c_init_pre(int bus) {

    mraa_result_t ret = MRAA_ERROR_INVALID_PARAMETER;
    // In ROScube Pico NX, I2C0 mapped to /dev/i2c-1, I2C1 mapped to /dev/i2c-8
    int internal_bus = 0;
    if (bus == 0) {
        internal_bus = 1; 
    } else if (bus == 1) {
        internal_bus = 8;
    } else {
        return MRAA_ERROR_INVALID_PARAMETER;
    }

    char devpath[MAX_DEVPATH_LENGTH];
    sprintf(devpath, MAX_DEVPATH_LENGTH, "/dev/i2c-%u", plat->i2c_bus[bus].bus_id);
    if (!mraa_file_exist(devpath)) {
        syslog(LOG_ERR, "i2c: %s doesn't exist.", devpath);
        return ret;
    }
    return MRAA_SUCCESS;
}

mraa_board_t* mraa_roscube_pico_nx() {

    mraa_result_t ret = MRAA_ERROR_UNSPECIFIED;

    mraa_board_t* board = (mraa_board_t *) calloc (1, sizeof(mraa_board_t));

    if (board == NULL) {
        goto error;
    }

    board->platform_name = MRAA_ROSCUBE_PICO_NX_BOARD_NAME;
    board->phy_pin_count = MRAA_ROSCUBE_PICO_NX_PINCOUT;

error:
    free(board);
    return NULL;
}
