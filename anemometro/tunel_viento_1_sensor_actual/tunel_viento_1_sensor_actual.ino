#include "Wire.h"
#include "ms4525do.h"  // https://github.com/bolderflight/ms4525do

#define TCAADDR 0x70      // Dirección I2C del multiplexor TCA9548A
#define PRESADDR 0x28     // Dirección I2C del sensor MS4525DO
#define RANGE 1.0f        // Rango de medición en psi

bfs::Ms4525do sensor;     // Crear un único objeto para el sensor

// Función para seleccionar el canal del multiplexor
void tcaselect(uint8_t canal) {
  if (canal > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << canal); // Selecciona el canal indicado
  Wire.endTransmission();
}

// Inicializar el sensor
void setup_sensor(bfs::Ms4525do &sensor) {
  sensor.Config(&Wire, PRESADDR, RANGE, -RANGE);
  if (!sensor.Begin()) {
    Serial.println("Error comunicando con el sensor");
  }
}

// Setup
void setup() {
  Wire.begin();
  Serial.begin(9600);

  tcaselect(0);            // Selecciona el canal 0 del multiplexor
  setup_sensor(sensor);    // Inicializa el sensor
}

// Loop principal
void loop() {
  tcaselect(0);            // Asegura que estás en el canal correcto

  if (sensor.Read()) {
    Serial.print(sensor.pres_pa(), 6);       // Presión en Pascales
    Serial.print(",");
    Serial.println(sensor.die_temp_c(), 6);  // Temperatura en °C
  }

  delay(100);  // Espera para no saturar la salida serial
}
