#include "Wire.h"
#include "ms4525do.h" // link descarga librería: https://github.com/bolderflight/ms4525do

#define TCAADDR 0x70 //dirección I2C del multiplexor TCA
#define PRESADDR 0x28 //dirección I2C del sensor de presión diferencial
#define RANGE 1.0f //rango de presión sensor de presión diferencial

//Se definen objetos de la clase MS4525DO
bfs::Ms4525do sensor1; 
bfs::Ms4525do sensor2;  
bfs::Ms4525do sensor3;
bfs::Ms4525do sensor4;

// función para inicializar sensor
void setup_sensor(bfs::Ms4525do& sensor, int sensor_number) {
  sensor.Config(&Wire, PRESADDR, RANGE, -RANGE); // se indica la dirección 0x28 del sensor y los rangos de medición (-1psi, 1psi)
  if (!sensor.Begin()) {
    Serial.print("Error communicating with sensor ");
    Serial.println(sensor_number);
  }
}

// función para leer e imprimir los datos de presión y temperatura
void read_sensor(bfs::Ms4525do& sensor, int sensor_number) {
  if (sensor.Read()) {
    Serial.print("Sensor ");
    Serial.print(sensor_number);
    
    Serial.print(" presion [Pa]: ");
    Serial.print(sensor.pres_pa(), 6); // se imprime el valor con 6 decimales
    Serial.print("\t");
    
    Serial.print("tempe [°C]: ");
    Serial.print(sensor.die_temp_c(), 6); // se imprime el valor con 6 decimales
    Serial.print("\n");
  }
}

// función para seleccionar el bus I2C del multiplexor
void tcaselect(uint8_t i) {
  if (i > 7) return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void setup() {
  Wire.begin();
  Serial.begin(9600);

  tcaselect(0); // se selecciona el bus 0 del multiplexor
  setup_sensor(sensor1, 1);  // llama a la función setup_sensor para inicializar el sensor 1

  tcaselect(1); // se selecciona el bus 1 del multiplexor
  setup_sensor(sensor2, 2);  // llama a la función setup_sensor para inicializar el sensor 2

  tcaselect(2); // se selecciona el bus 2 del multiplexor
  setup_sensor(sensor3, 3);  // llama a la función setup_sensor para inicializar el sensor 3

  tcaselect(3); // se selecciona el bus 3 del multiplexor
  setup_sensor(sensor4, 4);  // llama a la función setup_sensor para inicializar el sensor 4
}

void loop() {
  tcaselect(0); // se selecciona el bus 0 del multiplexor
  read_sensor(sensor1, 1); // llama la función read_sensor para leer los datos del sensor 1
  
  tcaselect(1); // se selecciona el bus 1 del multiplexor
  read_sensor(sensor2, 2); // llama la función read_sensor para leer los datos del sensor 2
  
  tcaselect(2); // se selecciona el bus 2 del multiplexor
  read_sensor(sensor3, 3); // llama la función read_sensor para leer los datos del sensor 3
  
  tcaselect(3); // se selecciona el bus 3 del multiplexor
  read_sensor(sensor4, 4); // llama la función read_sensor para leer los datos del sensor 4
}
