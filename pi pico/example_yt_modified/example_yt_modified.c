#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0
#define BNO055_ADDR 0x28
#define SYS_TRIGGER_REGISTER 0x3F
#define PWR_MODE_REGISTER 0x3E
#define AXIS_MAP_CONFIG_REGISTER 0x41
#define AXIS_MAP_SIGN_REGISTER 0x42
#define UNIT_SEL_REGISTER 0x3B
#define OPR_MODE_REGISTER 0x3D

//static int BNO055_ADDR = 0x28;

// Initialise Accelerometer Function
void imu_setting(void){
    // Check to see if connection is correct
    sleep_ms(1000); // Add a short delay to help BNO005 boot up
    uint8_t reg = 0x00;
    uint8_t chipID[1];
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, BNO055_ADDR, chipID, 1, false);

    if(chipID[0] != 0xA0){
        while(1){
            printf("Chip ID Not Correct - Check Connection!");
            sleep_ms(5000);
        }
    }

    // Use internal oscillator
    uint8_t data[2];
    data[0] = SYS_TRIGGER_REGISTER; //SYS_TRIGGER
    data[1] = 0x40;
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, data, 2, true);

    // Reset all interrupt status bits
    data[0] = SYS_TRIGGER_REGISTER; //SYS_TRIGGER
    data[1] = 0x01; //? no se reconce
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, data, 2, true);

    // Configure Power Mode
    data[0] = PWR_MODE_REGISTER; //PWR_MODE
    data[1] = 0x00; //Normal Mode
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, data, 2, true);
    sleep_ms(50);

    // Defaul Axis Configuration
    data[0] = AXIS_MAP_CONFIG_REGISTER; //AXIS_MAP_CONFIG
    data[1] = 0x24; //X -> X, Y -> Y, Z -> Z (default)
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, data, 2, true);

    // Default Axis Signs
    data[0] = AXIS_MAP_SIGN_REGISTER; //AXIS_MAP_SIGN
    data[1] = 0x00; //0b111; //0x00; //0 da al reves
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, data, 2, true);

    // Set units to m/s^2
    data[0] = UNIT_SEL_REGISTER; //UNIT_SEL
    data[1] = 0b0001000; //m/s^2, deg/s, deg, C, windows
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, data, 2, true);
    sleep_ms(30);

    // Set operation to NDOF
    data[0] = OPR_MODE_REGISTER; // OPR_MODE
    data[1] = 0x0C; //NDOF
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, data, 2, true);
    sleep_ms(100);
}

int main(void){
    stdio_init_all(); // Initialise STD I/O for printing over serial

    // Configure the I2C Communication
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(21, GPIO_FUNC_I2C);
    gpio_set_function(20, GPIO_FUNC_I2C);
    gpio_pull_up(21);
    gpio_pull_up(20);

    // Call accelerometer initialization function
    imu_setting();

    uint8_t accel[6]; // Store data from the 6 acceleration registers
    uint8_t lin_accel[6]; // Store data from the 6 linear acceleration registers
    uint8_t gyro[6]; // Store data from the 6 gyroscope registers
    uint8_t mag[6]; // Store data from the 6 magnetometer registers
    uint8_t euler[6]; // Store data from the 6 euler angle registers
    uint8_t quat[8]; // Store data from the 8 quaternion registers
    uint8_t calib;

    int16_t accelX, accelY, accelZ; // Combined 3 axis data
    int16_t lin_accelX, lin_accelY, lin_accelZ; // Combined 3 axis data
    int16_t gyroX, gyroY, gyroZ; // Combined 3 axis data
    int16_t magX, magY, magZ; // Combined 3 axis data
    int16_t eulerX, eulerY, eulerZ; // Combined 3 axis data
    int16_t quatW, quatX, quatY, quatZ; // Combined axis data

    float f_accelX, f_accelY, f_accelZ; // Float type of acceleration data
    float f_lin_accelX, f_lin_accelY, f_lin_accelZ; // Float type of linear acceleration data
    float f_gyroX, f_gyroY, f_gyroZ; // Float type of gyroscope data
    float f_magX, f_magY, f_magZ; // Float type of magnetometer data
    float f_eulerX, f_eulerY, f_eulerZ; // Float type of euler angle data
    float f_quatW, f_quatX, f_quatY, f_quatZ; // Float type of quaternion data


    uint8_t ACC_DATA_X_LSB = 0x08;
    uint8_t LIA_DATA_X_LSB = 0x28;
    uint8_t GYR_DATA_X_LSB = 0x14;
    uint8_t MAG_DATA_X_LSB = 0x0E;
    uint8_t EUL_HEADING_LSB = 0x1A;
    uint8_t QUAT_W_LSB = 0x20;
    uint8_t CALIB_STATb = 0x35;


    // Infinite Loop
    while(1){
        i2c_write_blocking(I2C_PORT, BNO055_ADDR, &CALIB_STATb, 1, true);
        i2c_read_blocking(I2C_PORT, BNO055_ADDR, &calib, 1, false);
        uint8_t sys_calib = (calib >> 6) & 0x03;  // Bits [7:6]
        uint8_t gyro_calib = (calib >> 4) & 0x03; // Bits [5:4]
        uint8_t acc_calib = (calib >> 2) & 0x03;  // Bits [3:2]
        uint8_t mag_calib = calib & 0x03;         // Bits [1:0]

        // Imprime los estados de calibración
        printf("Calibración: Sistema=%d, Giroscopio=%d, Acelerómetro=%d, Magnetómetro=%d\n",
               sys_calib, gyro_calib, acc_calib, mag_calib);
        //
        i2c_write_blocking(I2C_PORT, BNO055_ADDR, &ACC_DATA_X_LSB, 1, true);
        i2c_read_blocking(I2C_PORT, BNO055_ADDR, accel, 6, false);

        accelX = ((accel[1]<<8) | accel[0]);
        accelY = ((accel[3]<<8) | accel[2]);
        accelZ = ((accel[5]<<8) | accel[4]);

        f_accelX = accelX / 100.00;
        f_accelY = accelY / 100.00;
        f_accelZ = accelZ / 100.00;
        //
        i2c_write_blocking(I2C_PORT, BNO055_ADDR, &LIA_DATA_X_LSB, 1, true);
        i2c_read_blocking(I2C_PORT, BNO055_ADDR, lin_accel, 6, false);

        lin_accelX = ((lin_accel[1]<<8) | lin_accel[0]);
        lin_accelY = ((lin_accel[3]<<8) | lin_accel[2]);
        lin_accelZ = ((lin_accel[5]<<8) | lin_accel[4]);

        f_lin_accelX = lin_accelX / 100.00;
        f_lin_accelY = lin_accelY / 100.00;
        f_lin_accelZ = lin_accelZ / 100.00;
        //

        i2c_write_blocking(I2C_PORT, BNO055_ADDR, &GYR_DATA_X_LSB, 1, true);
        i2c_read_blocking(I2C_PORT, BNO055_ADDR, gyro, 6, false);

        gyroX = ((gyro[1]<<8) | gyro[0]);
        gyroY = ((gyro[3]<<8) | gyro[2]);
        gyroZ = ((gyro[5]<<8) | gyro[4]);

        f_gyroX = gyroX / 16.00;
        f_gyroY = gyroY / 16.00;
        f_gyroZ = gyroZ / 16.00;
        //
        i2c_write_blocking(I2C_PORT, BNO055_ADDR, &MAG_DATA_X_LSB, 1, true);
        i2c_read_blocking(I2C_PORT, BNO055_ADDR, mag, 6, false);
        magX = ((mag[1]<<8) | mag[0]);
        magY = ((mag[3]<<8) | mag[2]);
        magZ = ((mag[5]<<8) | mag[4]);

        f_magX = magX / 16.00;
        f_magY = magY / 16.00;
        f_magZ = magZ / 16.00;
        //
        i2c_write_blocking(I2C_PORT, BNO055_ADDR, &EUL_HEADING_LSB, 1, true);
        i2c_read_blocking(I2C_PORT, BNO055_ADDR, euler, 6, false);
        eulerX = ((euler[1]<<8) | euler[0]);
        eulerY = ((euler[3]<<8) | euler[2]);
        eulerZ = ((euler[5]<<8) | euler[4]);

        f_eulerX = eulerX / 16.00;
        f_eulerY = eulerY / 16.00;
        f_eulerZ = eulerZ / 16.00;

        //
        i2c_write_blocking(I2C_PORT, BNO055_ADDR, &QUAT_W_LSB, 1, true);
        i2c_read_blocking(I2C_PORT, BNO055_ADDR, quat, 8, false);

        quatW = ((quat[1]<<8) | quat[0]);
        quatX = ((quat[3]<<8) | quat[2]);
        quatY = ((quat[5]<<8) | quat[4]);
        quatZ = ((quat[7]<<8) | quat[6]);

        f_quatW = quatW / 16384.00;
        f_quatX = quatX / 16384.00;
        f_quatY = quatY / 16384.00;
        f_quatZ = quatZ / 16384.00;

        // Print to serial monitor
        printf("Accel  X: %6.2f    Y: %6.2f    Z: %6.2f\n", f_accelX, f_accelY, f_accelZ);
        printf("LinAcc X: %6.2f    Y: %6.2f    Z: %6.2f\n", f_lin_accelX, f_lin_accelY, f_lin_accelZ);
        printf("Gyro   X: %6.2f    Y: %6.2f    Z: %6.2f\n", f_gyroX, f_gyroY, f_gyroZ);
        printf("Mag    X: %6.2f    Y: %6.2f    Z: %6.2f\n", f_magX, f_magY, f_magZ);
        printf("Euler  X: %6.2f    Y: %6.2f    Z: %6.2f\n", f_eulerX, f_eulerY, f_eulerZ);
        printf("Quat   W: %6.2f    X: %6.2f    Y: %6.2f    Z: %6.2f\n", f_quatW, f_quatX, f_quatY, f_quatZ);
        printf("\n");
        sleep_ms(300);
    }
}
