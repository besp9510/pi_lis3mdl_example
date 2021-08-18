// LIS3MDL register address, defaults, and masks from data sheet 

// Register addresses (page 23):
#define WHO_AM_I 0x0F   // Device identification register
#define CTRL_REG1 0x20  // Temp enable, data rate, op mode, self-test
#define CTRL_REG2 0x21  // Reboot and soft reset
#define CTRL_REG3 0x22  // Low power select, SPI mode, operation mode
#define CTRL_REG4 0x23  // Z axis op mode, endian data select
#define CTRL_REG5 0x24  // FAST READ, block data update
#define STATUS_REG 0x27 // ZYXOR through XDA data overrun/available
#define OUT_X_L 0x28    // X-axis data output (two's complement)
#define OUT_X_H 0x29    // X-axis data output (two's complement)
#define OUT_Y_L 0x2A    // Y-axis data output (two's complement)
#define OUT_Y_H 0x2B    // Y-axis data output (two's complement)
#define OUT_Z_L 0x2C    // Z-axis data output (two's complement)
#define OUT_Z_H 0x2D    // Z-axis data output (two's complement)
#define TEMP_OUT_L 0x2E // Temperature data output (two's complement)
#define TEMP_OUT_H 0x2F // Temperature data output (two's complement)
#define INT_CFG 0x30    // Interrupt generation enable
#define INT_SRC 0x31    // Exceed threshold, interrupt occurred?
#define INT_THS_L 0x32  // Interupt threshold
#define INT_THS_H 0x33  // Interupt threshold

// Register defaults (page 23):
#define WHO_AM_I_DEFALT 0x3D  // (= 00111101)
#define CTRL_REG1_DEFALT 0x10 // (= 00010000)
#define CTRL_REG2_DEFALT 0x00 // (= 00000000)
#define CTRL_REG3_DEFALT 0x03 // (= 00000011)
#define CTRL_REG4_DEFALT 0x00 // (= 00000000)
#define CTRL_REG5_DEFALT 0x00 // (= 00000000)
#define INT_CFG_DEFALT 0xE8   // (= 11101000)
#define INT_SRC_DEFALT 0x00   // (= 00000000)
#define INT_THS_L_DEFALT 0x00 // (= 00000000)
#define INT_THS_H_DEFALT 0x00 // (= 00000000)

// Register setting masks (page 24-30):
#define X_Y_AXES_LOW_POWER (0x0 << 5)
#define X_Y_AXES_MED_PERF (0x01 << 5)
#define X_Y_AXES_HIGH_PERF (0x02 << 5)
#define X_Y_AXES_ULTRA_PERF (0x03 << 5)

#define Z_AXES_LOW_POWER (0x0 << 2)
#define Z_AXES_MED_PERF (0x01 << 2)
#define Z_AXES_HIGH_PERF (0x02 << 2)
#define Z_AXES_ULTRA_PERF (0x03 << 2)

#define DATA_RATE_0_625 (0x0 << 2)
#define DATA_RATE_1_25 (0x01 << 2)
#define DATA_RATE_2_5 (0x02 << 2)
#define DATA_RATE_5 (0x03 << 2)
#define DATA_RATE_10 (0x04 << 2)
#define DATA_RATE_20 (0x05 << 2)
#define DATA_RATE_40 (0x06 << 2)
#define DATA_RATE_80 (0x07 << 2)

#define SENSITIVITY_4_G (0x0 << 5)
#define SENSITIVITY_8_G (0x01 << 5)
#define SENSITIVITY_12_G (0x02 << 5)
#define SENSITIVITY_16_G (0x03 << 5)

#define CONTINUOUS_CONVERSION_MODE 0x0
#define SINGLE_CONVERSION_MODE 0x01
#define POWER_DOWN_MODE 0x02