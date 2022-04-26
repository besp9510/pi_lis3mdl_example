// Raspberry Pi LIS3MDL Example
//
// Copyright (c) 2021 Benjamin Spencer
// ============================================================================
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// ============================================================================

// Include C standard libraries:
#include <stdlib.h> // C Standard library
#include <stdio.h>  // C Standard I/O libary
#include <time.h>   // C Standard date and time manipulation
#include <stdint.h> // C Standard integer types

#include <pi_i2c.h>     // Pi I2C library!
#include <pi_lw_gpio.h> // Pi GPIO library!

#include "lis3mdl_registers.h"

#define DEVICE_POWER_GPIO 4

// Testing LIS3MDL "Digital output magnetic sensor: ultra-low-power,
// high-performance 3-axis magnetometer" per the datasheet:
// (Can find under doc/lis3mdl.pdf)

int reboot_device(void) {
    printf("Attempting to reboot the device!\n");

    // Toggle GPIO off and on to reboot device:
    gpio_clear(DEVICE_POWER_GPIO);
    sleep(1);
    gpio_set(DEVICE_POWER_GPIO);

    printf("Reboot done\n");
}

int i2c_error_handler(int errno) {
    // An I2C error may be fatal or maybe something that we can recover from
    // or even ignore all together:
    switch (errno) {
        case -ENACK:
            printf("I2C Error! Encountered ENACK\n");
            // Try rebooting device:
            reboot_device();
            break;
        case -EBADXFR:
            printf("I2C Error! Encountered EBADXFR\n");
            // Try rebooting device:
            reboot_device();
            break;
        case -EBADREGADDR:
            printf("I2C Error! Encountered EBADREGADDR\n");
            // Try rebooting device:
            reboot_device();
            break;
        case -ECLKTIMEOUT:
            printf("I2C Error! Encountered ECLKTIMEOUT\n");
            // Try rebooting device:
            reboot_device();
            break;
        case -ENACKRST:
            printf("I2C Error! Encountered ENACKRST\n");
            // Try rebooting device:
            reboot_device();
            break;
        case -EBUSLOCKUP:
            printf("I2C Error! Encountered EBUSLOCKUP\n");
            // Try rebooting device:
            reboot_device();
            break;
        case -EBUSUNKERR:
            printf("I2C Error! Encountered EBUSUNKERR\n");
            // Try rebooting device:
            reboot_device();
            break;
        case -EFAILSTCOND:
            printf("I2C Error! Encountered EFAILSTCOND\n");
            // Try rebooting device:
            reboot_device();
            break;
        case -EDEVICEHUNG:
            printf("I2C Error! Encountered EDEVICEHUNG\n");
            // Try rebooting device:
            reboot_device();
            break;
        default:
            break;
    }

    return 0;
}

int scan_for_device(uint8_t device_address) {
    // Address book passed to returned by the function:
    int address_book[127];

    int ret;

    if ((ret = scan_bus_i2c(address_book)) < 0) {
        i2c_error_handler(ret);
    }

    // Check and see if LIS3MDL was detected on the bus (if not then we can't
    // really continue with the test):
    if (address_book[device_address] != 1) {
        printf("Device was not detected at 0x%X\n", device_address);
        return -1;
    }

    printf("Device was detected at 0x%X\n", device_address);

    return 0;
}

int verify_device_id(uint8_t device_address) {
    // The returned device ID will be stored into an array that we pass
    // to the read function:
    int device_id[1];

    int ret;

    if ((ret = read_i2c(device_address, WHO_AM_I, device_id, 0x01)) < 0) {
        i2c_error_handler(ret);
    }

    // Compare returned device ID and error out if it does not match expected
    // (If it doesn't match I would suspect something has gone horribly wrong)
    if (device_id[0] != WHO_AM_I_DEFALT) {
        printf("Device identity (0x%X) does not match expected (0x%X)\n",
               device_id[0], WHO_AM_I_DEFALT);
        return -1;
    }

    printf("Device identity (0x%X) matches expected (0x%X)\n",
            device_id[0], WHO_AM_I_DEFALT);

    return 0;
}

int configure_device(uint8_t device_address, int xy_ops_mode, int z_ops_mode,
                     int data_rate, int sensitivity) {
    // Device settings will be retrieved, stored, and modified in an array
    // that will be passed to read and write:
    int ctrl_reg1_value[1] = {0};
    int ctrl_reg2_value[1] = {0};
    int ctrl_reg3_value[1] = {0};
    int ctrl_reg4_value[1] = {0};
    int ctrl_reg5_value[1] = {0};

    int ret;

    printf("Reading device control registers\n");

    if ((ret = read_i2c(device_address, CTRL_REG1, ctrl_reg1_value, 0x01)) < 0) {
        i2c_error_handler(ret);
    }

    if ((ret = read_i2c(device_address, CTRL_REG2, ctrl_reg2_value, 0x01)) < 0) {
        i2c_error_handler(ret);
    }

    if ((ret = read_i2c(device_address, CTRL_REG4, ctrl_reg4_value, 0x01)) < 0) {
        i2c_error_handler(ret);
    }

    printf("Current configs: ctrl_reg1 = 0x%X\n", ctrl_reg1_value[0]);
    printf("Current configs: ctrl_reg2 = 0x%X\n", ctrl_reg2_value[0]);
    printf("Current configs: ctrl_reg3 = 0x%X\n", ctrl_reg3_value[0]);
    printf("Current configs: ctrl_reg4 = 0x%X\n", ctrl_reg4_value[0]);
    printf("Current configs: ctrl_reg5 = 0x%X\n", ctrl_reg5_value[0]);

    ctrl_reg1_value[0] = (ctrl_reg1_value[0] & 0x83) | (xy_ops_mode | data_rate);
    ctrl_reg2_value[0] = (ctrl_reg2_value[0] & 0x9F) | (sensitivity);
    ctrl_reg4_value[0] = (ctrl_reg4_value[0] & 0xF3) | (z_ops_mode);

    printf("Setting device configs: ctrl_reg1 = 0x%X\n", ctrl_reg1_value[0]);
    printf("Setting device configs: ctrl_reg2 = 0x%X\n", ctrl_reg2_value[0]);
    printf("Setting device configs: ctrl_reg4 = 0x%X\n", ctrl_reg4_value[0]);

    if ((ret = write_i2c(device_address, CTRL_REG1, ctrl_reg1_value, 0x01)) < 0) {
        i2c_error_handler(ret);
    }

    if ((ret = write_i2c(device_address, CTRL_REG2, ctrl_reg2_value, 0x01)) < 0) {
        i2c_error_handler(ret);
    }

    if ((ret = write_i2c(device_address, CTRL_REG4, ctrl_reg4_value, 0x01)) < 0) {
        i2c_error_handler(ret);
    }

    printf("Device configured\n");

    return 0;
}

int select_operating_mode(uint8_t device_address, int mode) {
    // Device settings will be retrieved, stored, and modified in an array
    // that will be passed to read and write:
    int ctrl_reg3_value[1] = {0};

    int ret;

    printf("Configuring operating mode\n");

    if ((ret = read_i2c(device_address, CTRL_REG3, ctrl_reg3_value, 0x01)) < 0) {
        i2c_error_handler(ret);
    }

    printf("Current configs: ctrl_reg3 = 0x%X\n", ctrl_reg3_value[0]);

    ctrl_reg3_value[0] = (ctrl_reg3_value[0] & 0xFC) | mode;

    printf("Setting device configs: ctrl_reg3 = 0x%X\n", ctrl_reg3_value[0]);

    if ((ret = write_i2c(device_address, CTRL_REG3, ctrl_reg3_value, 0x01)) < 0) {
        i2c_error_handler(ret);
    }

    printf("Device configured\n");

    return 0;
}

int read_mag_data(uint8_t device_address, int sensitivity, float *mag_nTesla) {
    int temp[6] = {0, 0, 0, 0, 0, 0};

    int16_t mag_x_raw = 0;
    int16_t mag_y_raw = 0;
    int16_t mag_z_raw = 0;

    float mag_x_gauss = 0;
    float mag_y_gauss = 0;
    float mag_z_gauss = 0;

    float mag_x_nTesla = 0;
    float mag_y_nTesla = 0;
    float mag_z_nTesla = 0;

    int ret;

    int scale;

    // LSB per gauss:
    if (sensitivity == SENSITIVITY_16_G) {
        scale = 1711;
    } else if (sensitivity == SENSITIVITY_12_G) {
        scale = 2281;
    } else if (sensitivity == SENSITIVITY_8_G) {
        scale = 3421;
    } else if (sensitivity == SENSITIVITY_4_G) {
        scale = 6842;
    }

    printf("Reading magnetic field\n");

    if ((ret = read_i2c(device_address, OUT_X_L, temp, 6)) < 0) {
        i2c_error_handler(ret);
    }

    mag_x_raw = (temp[1] << 0x08) | temp[0];
    mag_y_raw = (temp[3] << 0x08) | temp[2];
    mag_z_raw = (temp[5] << 0x08) | temp[4];

    mag_x_gauss = (float) mag_x_raw / scale;
    mag_y_gauss = (float) mag_y_raw / scale;
    mag_z_gauss = (float) mag_z_raw / scale;

    mag_x_nTesla = (mag_x_gauss / 10000) * 1e9;
    mag_y_nTesla = (mag_y_gauss / 10000) * 1e9;
    mag_z_nTesla = (mag_z_gauss / 10000) * 1e9;

    printf("Magnetic field = [%.3f, %.3f, %.3f] gauss / [%.3f, %.3f, %.3f] nT\n",
           mag_x_gauss, mag_y_gauss, mag_z_gauss, mag_x_nTesla, mag_y_nTesla,
           mag_z_nTesla);

    mag_nTesla[0] = mag_x_nTesla;
    mag_nTesla[1] = mag_y_nTesla;
    mag_nTesla[2] = mag_z_nTesla;

    return 0;
}

int get_status(uint8_t device_address) {
    uint8_t status_value[1] = {0};

    // Bit fields found in the status register description: 
    int zyxor; // X, Y, Z-axis... Data overwritten?
    int zor;   // Z-axis... Data overwritten?
    int yor;   // Y-axis... Data overwritten?
    int xor;   // X-axis... Data overwritten?
    int zyxda; // X, Y, Z-axis... New data available?
    int zda;   // Z-axis... New data available?
    int yda;   // Y-axis... New data available?
    int xda;   // X-axis... New data available?

    int ret;

    printf("Reading device status register\n");

    if ((ret = read_i2c(device_address, STATUS_REG, status_value, 0x01)) < 0) {
        i2c_error_handler(ret);
    }

    // Segregate each bit field:
    zyxor = (status_value[0] & 0x80) >> 7;
    zor = (status_value[0] & 0x40) >> 6;
    yor = (status_value[0] & 0x20) >> 5;
    xor = (status_value[0] & 0x10) >> 4;
    zyxda = (status_value[0] & 0x08) >> 3;
    zda = (status_value[0] & 0x04) >> 2;
    yda = (status_value[0] & 0x02) >> 1;
    xda = (status_value[0] & 0x01);

    printf("Status register: zyxor = %d\n", zyxor);
    printf("Status register: zor   = %d\n", zor);
    printf("Status register: yor   = %d\n", yor);
    printf("Status register: xor   = %d\n", xor);
    printf("Status register: zyxda = %d\n", zyxda);
    printf("Status register: zda   = %d\n", zda);
    printf("Status register: yda   = %d\n", yda);
    printf("Status register: xda   = %d\n", xda);

    return status_value[0];
}

int main(void) {
    // LIS3MDL device address (page 17):
    uint8_t lis3mdl_addr = 0x1C;

    // Use the default I2C pins:
    // Ensure that Raspian I2C interface is disabled via rasp-config otherwise
    // risk unpredictable behavior!
    int sda_pin = 2; // UPDATE
    int scl_pin = 3; // UPDATE

    int speed_grade = I2C_FULL_SPEED;

    int ret;

    float mag_nTesla[3];

    int samples = 10;
    int i;

    printf("Begin test_lis3mdl.c\n");
    printf("Configuring pi_i2c:\n");
    printf("sda_pin = %d\n", sda_pin);
    printf("sda_pin = %d\n", scl_pin);
    printf("speed_grade = %d Hz\n", speed_grade);

    // Configure at standard mode:
    if ((ret = config_i2c(sda_pin, scl_pin, speed_grade)) < 0 ) {
        printf("config_i2c() failed to configure and returned %d\n", ret);
        return ret;
    }

    // Check to see if the device is present prior to interacting with device:
    if ((ret = scan_for_device(lis3mdl_addr)) < 0) {
        return -1;
    }

    // Check to see if the device ID matches what's expected prior to
    // continuing with the test: 
    if ((ret = verify_device_id(lis3mdl_addr)) < 0) {
        return -1;
    }

    // Set the following settings:
    // - X, Y, & Z axes high-performance mode
    // - Output data rate = 10 Hz
    // - Sensitivity = +/- 4 gauss
    if ((ret = configure_device(lis3mdl_addr, X_Y_AXES_HIGH_PERF,
                                Z_AXES_HIGH_PERF, DATA_RATE_10,
                                SENSITIVITY_4_G)) < 0) {
        return -1;
    }

    // Set the following settings:
    // - Continous conversion mode
    if ((ret = select_operating_mode(lis3mdl_addr,
                                     CONTINUOUS_CONVERSION_MODE)) < 0) {
        return -1;
    }

    if ((ret = get_status(lis3mdl_addr)) < 0) {
            return -1;
    }

    // Loop for some time to get magnetic field readings:
    for (i = 0; i < samples; i++) {
        if ((ret = read_mag_data(lis3mdl_addr,
                                 SENSITIVITY_4_G, mag_nTesla)) < 0) {
            return -1;
        }

        sleep(1);
    }

    return 0;
}
