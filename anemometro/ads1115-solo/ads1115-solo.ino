#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

// void signal_read(float volts0, float volts1, float volts2, float volts3, float adc0, float adc1, float adc2, float adc3){
//  //Lee la señal analógica
//  adc0 = ads.readADC_SingleEnded(0);
//  adc1 = ads.readADC_SingleEnded(1);
//  adc2 = ads.readADC_SingleEnded(2);
//  adc3 = ads.readADC_SingleEnded(3);

//  //Calcula el valor del voltaje de la señal analógica
//  volts0 = ads.computeVolts(adc0);
//  volts1 = ads.computeVolts(adc1);
//  volts2 = ads.computeVolts(adc2);
//  volts3 = ads.computeVolts(adc3);
// }
//
//void calculation(float volts0, float volts1, float volts2, float volts3, float adc0, float adc1, float adc2, float adc3){
//  // se calcula el valor de la diferencia de presión [Pa]
//  P0 = ((( volts0 - 0.04 * Vs ) / (0.09 * Vs))+ tolP);
//  P1 = ((( volts1 - 0.04 * Vs ) / (0.09 * Vs))+ tolP);
//  P2 = ((( volts2 - 0.04 * Vs ) / (0.09 * Vs))+ tolP);
//  P3 = ((( volts3 - 0.04 * Vs ) / (0.09 * Vs))+ tolP);
//
//  // se calcula la magnitud de la velocidad del viento [m/s]
//  V0 = sqrt(2 * P0/rho);
//  V1 = sqrt(2 * P1/rho);
//  V2 = sqrt(2 * P2/rho);
//  V3 = sqrt(2 * P3/rho);
//  
//  Serial.print("1. digital: "); Serial.print(adc0); Serial.print(" Voltaje: "); Serial.print(volts0); Serial.print(" ΔP0: "); Serial.print(P0); Serial.print("  "); Serial.print(" VEL0: "); Serial.println(V0);
//  Serial.print("2. digital: "); Serial.print(adc1); Serial.print(" Voltaje: "); Serial.print(volts1); Serial.print(" ΔP1: "); Serial.print(P1); Serial.print("  "); Serial.print("VEL1: "); Serial.println(V1);
//  Serial.print("3 .digital: "); Serial.print(adc2); Serial.print(" Voltaje: "); Serial.print(volts2); Serial.print(" ΔP2: "); Serial.print(P2); Serial.print("  "); Serial.print("VEL2: "); Serial.println(V2);
//  Serial.print("4. digital: "); Serial.print(adc3); Serial.print(" Voltaje: "); Serial.print(volts3); Serial.print(" ΔP3: "); Serial.print(P3); Serial.print("  "); Serial.print("VEL3: "); Serial.println(V3);
//}

void setup(void)
{
  Serial.begin(9600);
  
  ads.setGain(GAIN_TWOTHIRDS);  // 2/3 ganancia - voltaje de referencia: 6.144V - 1 bit = 0.1875mV

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
}

void loop(void)
{
  int16_t adc0, adc1, adc2, adc3; // se declaran como variables que representan números enteros de 16 bits con signo
  float volts0, volts1, volts2, volts3;

  // esta función lee la señal analógica
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);

  // esta función calcula el valor del voltaje de la señal analógica
  volts0 = ads.computeVolts(adc0);
  volts1 = ads.computeVolts(adc1);
  volts2 = ads.computeVolts(adc2);
  volts3 = ads.computeVolts(adc3);

  Serial.println("-----------------------------------------------------------");
  Serial.print("AIN0: "); Serial.print(adc0); Serial.print("  "); Serial.print(volts0); Serial.println("V");
  Serial.print("AIN1: "); Serial.print(adc1); Serial.print("  "); Serial.print(volts1); Serial.println("V");
  Serial.print("AIN2: "); Serial.print(adc2); Serial.print("  "); Serial.print(volts2); Serial.println("V");
  Serial.print("AIN3: "); Serial.print(adc3); Serial.print("  "); Serial.print(volts3); Serial.println("V");

}
