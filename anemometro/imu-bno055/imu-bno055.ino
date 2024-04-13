#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
  
Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup_imu();{
 if(!bno.begin()) // Si hay problemas detectando el sensor BNO055
  {
    Serial.print("Error iniciando IMU BNO055");
    while(1);
  }
  bno.setExtCrystalUse(true);
  displayCalStatus();
}

void displayCalStatus()
{
  // 3 significa totalmente calibrado
  // los datos deben ser igorados hasta que el sistema de calibración sea > 0
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);
  if (!system)
  {
    Serial.print("! ");
  }
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.println(mag, DEC);
}

void absolute_orientation(){
  sensors_event_t event; 
  bno.getEvent(&event);
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("Y: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("Z: ");
  Serial.println(event.orientation.z, 4);
}

void raw_euler_angles(){
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  Serial.print("X: ");
  Serial.print(euler.x());
  Serial.print(" Y: ");
  Serial.print(euler.y());
  Serial.print(" Z: ");
  Serial.print(euler.z());
  Serial.println("");
}

void raw_accelerometer(){
  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  Serial.print("X: ");
  Serial.print(accel.x());
  Serial.print(" Y: ");
  Serial.print(accel.y());
  Serial.print(" Z: ");
  Serial.print(accel.z());
  Serial.println("");
}

void raw_magnetometer(){
  imu::Vector<3> mag = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
  Serial.print("X: ");
  Serial.print(mag.x());
  Serial.print(" Y: ");
  Serial.print(mag.y());
  Serial.print(" Z: ");
  Serial.print(mag.z());
  Serial.println("");
}

void raw_gyroscope(){
  imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  Serial.print("X: ");
  Serial.print(gyro.x());
  Serial.print(" Y: ");
  Serial.print(gyro.y());
  Serial.print(" Z: ");
  Serial.print(gyro.z());
  Serial.println("");
}

void setup() 
{
  Serial.begin(9600);
  setup_imu();
}

void loop() 
{
  void absolute_orientation(); // Crea un evento y calcula la orientacion absoluta

  void raw_euler_angles(); // Mide los ángulos de Euler

  void raw_accelerometer(); // Mide la aceleración de cada eje

  void raw_magnetometer(); // Mide el campo magnético en cada eje

  void raw_gyroscope(); // Mide la velocidad angular de cada eje
}
