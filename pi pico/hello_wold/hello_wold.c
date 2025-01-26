#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    stdio_init_all(); // Inicializa la entrada/salida estándar
    while (!stdio_usb_connected()) {
        sleep_ms(100); // Espera 100 ms antes de reintentar
    }
    printf("inicio\n");
    printf("inicio\n");
    printf("inicio\n");
    printf("inicio\n");
    printf("inicio\n");
    printf("inicio\n");

    while (true) {
        printf("Hello, World!\n"); // Imprime en la consola serial
        sleep_ms(1000);           // Espera 1 segundo
    }

    return 0;
}
