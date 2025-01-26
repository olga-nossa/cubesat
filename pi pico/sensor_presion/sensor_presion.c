#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0
#define MS4525DO_ADDR 0x28 // Dirección del sensor

void read_sensor(uint16_t *pressure, uint16_t *temperature) {
    uint8_t buffer[4];

    // Leer 4 bytes del sensor
    i2c_read_blocking(I2C_PORT, MS4525DO_ADDR, buffer, 4, false);

    // Extraer los valores de presión y temperatura
    *pressure = ((buffer[0] & 0x3F) << 8) | buffer[1];
    *temperature = ((buffer[2] << 8) | buffer[3]) >> 5;
}

int main() {
    // Inicializar UART para depuración
    stdio_init_all();

    // Inicializar I²C
    i2c_init(I2C_PORT, 100 * 1000); // 100 kHz
    gpio_set_function(20, GPIO_FUNC_I2C); // SDA 20
    gpio_set_function(21, GPIO_FUNC_I2C); // SCL 21
    gpio_pull_up(20);
    gpio_pull_up(21);

    uint16_t pressure_raw, temperature_raw;
    float pressure, temperature;

    while (true) {
        // Leer los datos del sensor
        read_sensor(&pressure_raw, &temperature_raw);

        // Convertir presión
        //pressure = ((float)(pressure_raw - 0x0666) / (0x3999 - 0x0666)) * (1.0 - (-1.0)) + (-1.0);
        pressure = (((pressure_raw -1638.3)/6553.2) - 1.0) * 6895; //Se multiplica por 6895 para convertir de psi a Pa
        // Convertir temperatura
        temperature = (float)temperature_raw * 0.0977 - 50.0;

        // Imprimir resultados
        printf("Presión: %.6f, Temperatura: %.6f\n", pressure, temperature);
        printf("digital: %d, %d\n", pressure_raw, temperature_raw);

        sleep_ms(1000); // Esperar 1 segundo
    }

    return 0;
}
