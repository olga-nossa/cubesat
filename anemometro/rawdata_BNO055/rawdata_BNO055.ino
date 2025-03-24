#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define BNO055_SAMPLERATE_DELAY_MS (100) // With this delay the frecuency could be changed.

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28, &Wire);
void setup(void)
{
  Serial.begin(115200);

  while (!Serial) delay(10);  // wait for serial port to open!

  Serial.println("Orientation Sensor Raw Data Test"); Serial.println("");

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);

  bno.setExtCrystalUse(true);
}

void loop(void)
{
// Obtaining magnetic vector.
  imu::Vector<3> mag1 = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
  // All this part is only to print the values, the vector is saved as "mag1".
  Serial.print("X: ");
  Serial.print(mag1.x());
  Serial.print(" Y: ");
  Serial.print(mag1.y());
  Serial.print(" Z: ");
  Serial.print(mag1.z());
  Serial.print("\t");
  
  // Obtaining acceleration vector.
  imu::Vector<3> acce1 = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  // All this part is only to print the values, the vector is saved as "acce1".
  Serial.print("X: ");
  Serial.print(acce1.x());
  Serial.print(" Y: ");
  Serial.print(acce1.y());
  Serial.print(" Z: ");
  Serial.print(acce1.z());
  Serial.print("\t");

// Obtaining gyro vector.
  imu::Vector<3> gyro1 = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  // All this part is only to print the values, the vector is saved as "gyr01".
  Serial.print("X: ");
  Serial.print(gyro1.x());
  Serial.print(" Y: ");
  Serial.print(gyro1.y());
  Serial.print(" Z: ");
  Serial.print(gyro1.z());
  Serial.print("\t");

// This delay can be removed.
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
