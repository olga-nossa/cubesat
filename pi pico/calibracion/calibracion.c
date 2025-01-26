#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Direcciones y registros del BNO055
#define BNO055_ADDR 0x28
#define BNO055_OPR_MODE 0x3D
#define BNO055_ACC_OFFSET_X_LSB 0x55
#define NDOF_MODE 0x0C  // Modo de fusión
#define CONFIG_MODE 0x00  // Modo de configuración

// Función de escritura en I2C
void i2c_write(uint8_t reg, uint8_t *data, uint8_t length) {
    i2c_write_blocking(i2c0, BNO055_ADDR, &reg, 1, true);  // Dirección + Registro
    i2c_write_blocking(i2c0, BNO055_ADDR, data, length, false);  // Datos
}

// Función de lectura en I2C
void i2c_read(uint8_t reg, uint8_t *data, uint8_t length) {
    i2c_write_blocking(i2c0, BNO055_ADDR, &reg, 1, true);  // Dirección + Registro
    i2c_read_blocking(i2c0, BNO055_ADDR, data, length, false);  // Leer datos
}

// Función para cambiar el modo de operación
void set_operation_mode(uint8_t mode) {
    i2c_write(BNO055_OPR_MODE, &mode, 1);
    sleep_ms(20);  // Tiempo de espera para que el cambio de modo se efectúe correctamente
}

// Función para leer los datos de calibración
void read_calibration_data(uint8_t *calibration_data) {
    // Leer los registros de calibración (ajustar según la longitud necesaria)
    i2c_read(BNO055_ACC_OFFSET_X_LSB, calibration_data, 22);  // 22 bytes es un ejemplo
}

// Función para escribir los datos de calibración
void write_calibration_data(uint8_t *calibration_data) {
    i2c_write(BNO055_ACC_OFFSET_X_LSB, calibration_data, 22);  // Escribir los 22 bytes de calibración
}

int main() {
    // Inicializar I2C
    i2c_init(i2c0, 400000);  // Inicializa I2C con una velocidad de 400 kHz
    gpio_set_function(4, GPIO_FUNC_I2C);  // Configurar GPIO 4 para SDA
    gpio_set_function(5, GPIO_FUNC_I2C);  // Configurar GPIO 5 para SCL
    gpio_pull_up(4);  // Habilitar resistores de pull-up
    gpio_pull_up(5);  // Habilitar resistores de pull-up

    uint8_t calibration_data[22];  // Buffer para almacenar los datos de calibración

    // Paso 1: Cambiar al modo de configuración
    set_operation_mode(CONFIG_MODE);
    printf("Modo de configuración establecido.\n");

    // Paso 2: Leer los datos de calibración desde el sensor
    read_calibration_data(calibration_data);
    printf("Datos de calibración leídos.\n");

    // Paso 3: Guardar los datos de calibración para su reutilización (aquí puedes almacenarlos en memoria no volátil si es necesario)
    
    // Paso 4: Escribir los datos de calibración en el sensor
    write_calibration_data(calibration_data);
    printf("Datos de calibración escritos.\n");

    // Paso 5: Cambiar al modo de fusión para obtener orientación
    set_operation_mode(NDOF_MODE);
    printf("Modo de fusión establecido. Datos de orientación disponibles.\n");

    while (true) {
        // Aquí puedes obtener los datos de orientación, como el ángulo de Euler
        // y procesarlos según sea necesario
        sleep_ms(1000);  // Esperar 1 segundo antes de la siguiente lectura
    }

    return 0;
}