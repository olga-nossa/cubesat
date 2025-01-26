#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

//Definicion de los registros para configurar el sensor
#define I2C_PORT i2c0
#define BNO055_ADDR 0x28 //direccion de la IMU
#define CHIP_ID_REGISTER 0x00
#define SYS_TRIGGER_REGISTER 0x3F
#define PWR_MODE_REGISTER 0x3E
#define AXIS_MAP_CONFIG_REGISTER 0x41
#define AXIS_MAP_SIGN_REGISTER 0x42
#define UNIT_SEL_REGISTER 0x3B
#define OPR_MODE_REGISTER 0x3D

//Definicion de los registros para leer los datos del sensor
#define ACC_DATA_X_LSB 0x08
#define MAG_DATA_X_LSB 0x0E
#define GYR_DATA_X_LSB 0x14
#define EUL_HEADING_LSB 0x1A
#define QUAT_W_LSB 0x20
#define LIA_DATA_X_LSB 0x28
#define CALIB_STAT 0x35

//Funcion para iniciar el bus I2C
void init_i2c(void) {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(21, GPIO_FUNC_I2C);
    gpio_set_function(20, GPIO_FUNC_I2C);
    gpio_pull_up(21);
    gpio_pull_up(20);
}

//Funcion para configurar el sensor
void imu_setting(void) {
    uint8_t chipID[1];
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, (uint8_t[]){CHIP_ID_REGISTER}, 1, true);
    i2c_read_blocking(I2C_PORT, BNO055_ADDR, chipID, 1, false);

    if (chipID[0] != 0xA0) {
        while (1) {
            printf("Chip ID Not Correct - Check Connection!\n"); // Este condicional se puede borrar, es para las pruebas
            sleep_ms(5000);
        }
    }

    uint8_t data[2];
    data[0] = SYS_TRIGGER_REGISTER;
    data[1] = 0x40; // Use internal oscillator
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, data, 2, true);
    data[1] = 0x01; // Reset interrupts
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, data, 2, true);

    data[0] = PWR_MODE_REGISTER;
    data[1] = 0x00; // Normal power mode
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, data, 2, true);

    data[0] = AXIS_MAP_CONFIG_REGISTER;
    data[1] = 0x24; // Default axis mapping
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, data, 2, true);

    data[0] = AXIS_MAP_SIGN_REGISTER;
    data[1] = 0x00; // Default axis sign
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, data, 2, true);

    data[0] = UNIT_SEL_REGISTER;
    data[1] = 0b0001000; // Units in m/s^2
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, data, 2, true);

    data[0] = OPR_MODE_REGISTER;
    data[1] = 0x0C; // NDOF mode
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, data, 2, true);
}

//Funcion para ver el estado de calibracion, seguramente esta funcion se eliminara mas delante
void calib_status(void) {
    uint8_t calib;
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, (uint8_t[]){CALIB_STAT}, 1, true);
    i2c_read_blocking(I2C_PORT, BNO055_ADDR, &calib, 1, false);

    uint8_t sys_calib = (calib >> 6) & 0x03;  // Bits [7:6]
    uint8_t gyro_calib = (calib >> 4) & 0x03; // Bits [5:4]
    uint8_t acc_calib = (calib >> 2) & 0x03;  // Bits [3:2]
    uint8_t mag_calib = calib & 0x03;         // Bits [1:0]

    // Imprime los estados de calibración
    printf("Calibración: Sistema=%d, Giroscopio=%d, Acelerómetro=%d, Magnetómetro=%d\n", sys_calib, gyro_calib, acc_calib, mag_calib);
}

//Funcion para calcular aceleración
void read_acceleration(float *x, float *y, float *z) {
    uint8_t accel[6];
    int16_t rawX, rawY, rawZ;

    i2c_write_blocking(I2C_PORT, BNO055_ADDR, (uint8_t[]){ACC_DATA_X_LSB}, 1, true);
    i2c_read_blocking(I2C_PORT, BNO055_ADDR, accel, 6, false);

    rawX = ((accel[1]<<8) | accel[0]);
    rawY = ((accel[3]<<8) | accel[2]);
    rawZ = ((accel[5]<<8) | accel[4]);

    *x = rawX/100.00;
    *y = rawY/100.00;
    *z = rawZ/100.00;
}

//Funcion para leer y calcular aceleración lineal
void read_linear_acceleration(float *x, float *y, float *z) {
    uint8_t lin_accel[6];
    int16_t rawX, rawY, rawZ;

    i2c_write_blocking(I2C_PORT, BNO055_ADDR, (uint8_t[]){LIA_DATA_X_LSB}, 1, true);
    i2c_read_blocking(I2C_PORT, BNO055_ADDR, lin_accel, 6, false);

    rawX = (lin_accel[1]<<8) | lin_accel[0];
    rawY = (lin_accel[3]<<8) | lin_accel[2];
    rawZ = (lin_accel[5]<<8) | lin_accel[4];

    *x = rawX/100.00;
    *y = rawY/100.00;
    *z = rawZ/100.00;
}

//Funcion para leer y calcular giroscopio
void read_gyroscope(float *x, float *y, float *z) {
    uint8_t gyro[6];
    int16_t rawX, rawY, rawZ;

    i2c_write_blocking(I2C_PORT, BNO055_ADDR, (uint8_t[]){GYR_DATA_X_LSB}, 1, true);
    i2c_read_blocking(I2C_PORT, BNO055_ADDR, gyro, 6, false);

    rawX = (gyro[1]<<8) | gyro[0];
    rawY = (gyro[3]<<8) | gyro[2];
    rawZ = (gyro[5]<<8) | gyro[4];

    *x = rawX/16.00;
    *y = rawY/16.00;
    *z = rawZ/16.00;
}

//Funcion para leer y calcular magnetómetro
void read_magnetometer(float *x, float *y, float *z) {
    uint8_t mag[6];
    int16_t rawX, rawY, rawZ;

    i2c_write_blocking(I2C_PORT, BNO055_ADDR, (uint8_t[]){MAG_DATA_X_LSB}, 1, true);
    i2c_read_blocking(I2C_PORT, BNO055_ADDR, mag, 6, false);

    rawX = (mag[1]<<8) | mag[0];
    rawY = (mag[3]<<8) | mag[2];
    rawZ = (mag[5]<<8) | mag[4];

    *x = rawX/16.00;
    *y = rawY/16.00;
    *z = rawZ/16.00;
}

//Funcion para leer y calcular ángulos de Euler
void read_euler_angles(float *x, float *y, float *z) {
    uint8_t euler[6];
    int16_t rawX, rawY, rawZ;

    i2c_write_blocking(I2C_PORT, BNO055_ADDR, (uint8_t[]){EUL_HEADING_LSB}, 1, true);
    i2c_read_blocking(I2C_PORT, BNO055_ADDR, euler, 6, false);

    rawX = (euler[1]<<8) | euler[0];
    rawY = (euler[3]<<8) | euler[2];
    rawZ = (euler[5]<<8) | euler[4];

    *x = rawX/16.00;
    *y = rawY/16.00;
    *z = rawZ/16.00;
}

//Funcion para leer y calcular cuaterniones
void read_quaternion(float *w, float *x, float *y, float *z) {
    uint8_t quat[8];
    int16_t rawW, rawX, rawY, rawZ;

    i2c_write_blocking(I2C_PORT, BNO055_ADDR, (uint8_t[]){QUAT_W_LSB}, 1, true);
    i2c_read_blocking(I2C_PORT, BNO055_ADDR, quat, 8, false);

    rawW = (quat[1]<<8) | quat[0];
    rawX = (quat[3]<<8) | quat[2];
    rawY = (quat[5]<<8) | quat[4];
    rawZ = (quat[7]<<8) | quat[6];

    *w = rawW/16384.00;
    *x = rawX/16384.00;
    *y = rawY/16384.00;
    *z = rawZ/16384.00;
}

int main(void) {
    stdio_init_all();
    init_i2c();
    imu_setting();

    //variables para almacenar los valores del sensor
    float accelX, accelY, accelZ;
    float lin_accelX, lin_accelY, lin_accelZ;
    float gyroX, gyroY, gyroZ;
    float magX, magY, magZ;
    float eulerX, eulerY, eulerZ;
    float quatW, quatX, quatY, quatZ;

    while (1) {
        calib_status(); //esta funcion se eliminara mas adelante, es para verificar el estado de calibracion.
        read_acceleration(&accelX, &accelY, &accelZ);
        read_linear_acceleration(&lin_accelX, &lin_accelY, &lin_accelZ);
        read_gyroscope(&gyroX, &gyroY, &gyroZ);
        read_magnetometer(&magX, &magY, &magZ);
        read_euler_angles(&eulerX, &eulerY, &eulerZ);
        read_quaternion(&quatW, &quatX, &quatY, &quatZ);

        //imprimir resultados
        printf("Aceleración: X=%6.2f, Y=%6.2f, Z=%6.2f\n", accelX, accelY, accelZ);
        printf("Aceleración Lineal: X=%6.2f, Y=%6.2f, Z=%6.2f\n", lin_accelX, lin_accelY, lin_accelZ);
        printf("Giroscopio: X=%6.2f, Y=%6.2f, Z=%6.2f\n", gyroX, gyroY, gyroZ);
        printf("Magnetómetro: X=%6.2f, Y=%6.2f, Z=%6.2f\n", magX, magY, magZ);
        printf("Ángulos de Euler: X=%6.2f, Y=%6.2f, Z=%6.2f\n", eulerX, eulerY, eulerZ);
        printf("Cuaternión: W=%6.2f, X=%6.2f, Y=%6.2f, Z=%6.2f\n", quatW, quatX, quatY, quatZ);

        sleep_ms(500);
    }

    return 0;
}