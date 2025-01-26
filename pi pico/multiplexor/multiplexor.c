#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0          // Puerto I²C a usar
#define SDA_PIN 20              // Pin GPIO para SDA
#define SCL_PIN 21              // Pin GPIO para SCL
#define TCA9548_ADDR 0x70      // Dirección I²C del TCA9548

void init_i2c() {
    // Inicializa el bus I²C a 100 kHz
    i2c_init(I2C_PORT, 100 * 1000);

    // Configura los pines GPIO para funciones I²C
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    // Habilita resistencias pull-up internas para los pines SDA y SCL
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
}
// For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

void select_channel(uint8_t channel) {
    if (channel > 7) return; // Solo hay 8 canales (0-7)
    
    uint8_t data = 1 << channel; // Genera el byte para habilitar el canal
    
    // Envía el byte al multiplexor
    int result = i2c_write_blocking(I2C_PORT, TCA9548_ADDR, &data, 1, false);
    if (result == PICO_ERROR_GENERIC) {
        printf("Error al escribir en el TCA9548\n");
    }
}

// Función para escanear las direcciones I²C en el canal activo
void scan_channel(uint8_t channel) {
    bool found_device = false;
    printf("Escaneando el canal %d...\n", channel);

    // Escanea todas las direcciones válidas en I²C (0x03 a 0x77)
    for (uint8_t addr = 0x03; addr <= 0x77; addr++) {
        uint8_t data;
        int result = i2c_read_blocking(I2C_PORT, addr, &data, 1, false);
        if (result >= 0) { // Si se recibe respuesta, hay un dispositivo
            printf("   Dispositivo detectado en dirección: 0x%02X\n", addr);
            found_device = true;
        }
    }

    if (!found_device) {
        printf("   No se detectaron dispositivos en el canal %d\n", channel);
    }
    printf("\n");
}

int main()
{
    stdio_init_all(); // Inicializa el UART para depuración VER SI ES NECESARIO
    init_i2c();       // Configura el bus I²C

    while (true) {
        for (uint8_t channel = 0; channel < 8; channel++) {
            select_channel(channel); // Selecciona el canal
            scan_channel(channel);   // Escanea los dispositivos en el canal
            sleep_ms(1000);          // Espera 1 segundo
        }
        uint8_t data = 0x00; // Desactiva todos los canales
        i2c_write_blocking(I2C_PORT, TCA9548_ADDR, &data, 1, false);
    }
    return 0;
}