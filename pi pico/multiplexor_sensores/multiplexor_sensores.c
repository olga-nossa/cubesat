#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0 //Puerto I2C que se uso
#define SDA_PIN 20 //Pin GPIO para SDA
#define SCL_PIN 21 //Pin GPIO para SCL
#define TCA9548_ADDR 0x70 //Dirección I2C del multiplexor TCA9548
#define MS4525DO_ADDR 0x28 //Dirección I2C del sensor de presion MS4525DO

//Canales del multiplexor
 const uint8_t channels[] = {0,1,5,7}; //canales del multiplexor que se usaran separados por comas. De acuerdo a como sea mejor organizar el hardware se seleccionan los canales

//Funcion para inicializar el bus I2C
void init_i2c() {
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
}

//Funcion para seleccionar un canal del multiplexor
void select_channel(uint8_t channel) { //Entra como parametro un argumento de tipo uint8_t señalando el canal que se quiere seleccionar
    uint8_t data = 1 << channel; //Genera el byte para habilitar el canal. Ese byte lo indica el datasheet del TCA9548
    int result = i2c_write_blocking(I2C_PORT, TCA9548_ADDR, &data, 1, false); //Escribe en el TCA9548 para seleccionar el canal

    if (result == PICO_ERROR_GENERIC) {
        printf("Error al escribir en el TCA9548\n"); //Se usa para las pruebas, si hay un error al escribir en el multiplexor. Luego se puede quitar
    }
}

//Funcion para leer los datos del sensor de presion
void read_sensor(float *pressure, float *temperature) { //Funcion que lee los datos del sensor de presion. Entra como parametro dos punteros: presion y temperatura
    uint8_t buffer[4]; //Se declara un arreglo llamado buffer de 4 bytes para almacenar los datos leidos del sensor. 2 bytes para la presion y 2 bytes para la temperatura
    int result = i2c_read_blocking(I2C_PORT, MS4525DO_ADDR, buffer, 4, false); //Lee los datos del sensor y los almacena en buffer

    if (result < 0) {
        printf("Error al leer del sensor\n"); //Se usa para las pruebas, si hay un error al leer del sensor. Luego se puede quitar
        *pressure = 0.0;
        *temperature = 0.0;
        return;
    }

    // Extraer y convertir los valores de presión y temperatura
    uint16_t pressure_raw = ((buffer[0] & 0x3F) << 8) | buffer[1]; //Extrae los 2 bytes de la presion y los almacena en pressure_raw generando un numero en sistema decimal 
    /*  & 0x3F: elimina los dos bits mas significativos.
        << 8: desplaza el resultado 8 bits a la izquierda para colocarlo en la posición más significativa de un entero de 16 bits.
        | buffer[1]: Combina (mediante un OR bit a bit) el resultado anterior con el segundo byte del búfer (buffer[1]). */

    uint16_t temperature_raw = ((buffer[2] << 8) | buffer[3]) >> 5; //Extrae los 2 bytes de la temperatura y los almacena en temperature_raw generando un numero en sistema decimal
    /*  buffer[2]<< 8: toma el tercer byte de búfer y lo desplaza 8 bits a la izquierda, colocándolo en la posición más significativa de un entero de 16 bits.
        | buffer[3]: combina (mediante un OR bit a bit) el resultado anterior con el cuarto byte del búfer (buffer[3]).
        >> 5: desplaza el resultado 5 bits a la derecha, eliminando los 5 bits menos significativos. Esto se hace porque los datos útiles de temperatura están en los bits más significativos.  */

    *pressure = (((pressure_raw - 1638.3) / 6553.2) - 1.0) * 6895; //Con el valor obtenido anteriormente y con la funcion de transferencia se calcula presion en  PSI y luego se convierte a Pa
    *temperature = temperature_raw * 0.098 - 50.0; // Con el valor obtenido anteriormente y con la funcion de transferencia se calcula la temperatura en grados Celsius
}

int main() {
    stdio_init_all(); // Inicializa UART para depuración
    init_i2c(); //Funcion para configurar el bus I²C

    float pressure, temperature;

    while (true) {
        for (size_t i = 0; i < sizeof(channels) / sizeof(channels[0]); i++) { //Itera sobre los canales del multiplexor almacenados en el arreglo channels
            uint8_t channel = channels[i];
            select_channel(channel); //Funcion para selecciona el canal del multiplexor
            read_sensor(&pressure, &temperature); //Funcion para leer los datos del sensor
            printf("Canal: %d | Presión: %.6f Pa | Temperatura: %.6f C\n", channel, pressure, temperature); //Imprimir resultados con 6 decimales, se cambia de acuerdo a como se necesite
        }
        uint8_t data = 0x00; //Desactiva todos los canales. Esto se hace para evitar que se quede seleccionado un canal
        i2c_write_blocking(I2C_PORT, TCA9548_ADDR, &data, 1, false); //Escribe en el TCA9548 para desactivar todos los canales
    }
    return 0;
}