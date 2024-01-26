#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads;

double P0, P1, P2, P3, Vs = 5.0;
double tolP = 0.04; // Este valor se debe determinar
double V0, V1, V2, V3;
int rho = 1.225;

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

  // se calcula el valor de la diferencia de presión
  P0 = (( volts0 - 0.04 * Vs ) / (0.09 * Vs) + tolP) * 1000;
  P1 = (( volts1 - 0.04 * Vs ) / (0.09 * Vs) + tolP) * 1000;
  P2 = (( volts2 - 0.04 * Vs ) / (0.09 * Vs) + tolP) * 1000;
  P3 = (( volts3 - 0.04 * Vs ) / (0.09 * Vs) + tolP) * 1000;

  // se calcula la magnitud de la velocidad del viento
  V0 = sqrt(2 * P0/rho);
  V1 = sqrt(2 * P1/rho);
  V2 = sqrt(2 * P2/rho);
  V3 = sqrt(2 * P3/rho);
  
  Serial.print("ΔP0: "); Serial.print(P0); Serial.print("  "); Serial.print("VEL0: "); Serial.println(V0);
  Serial.print("ΔP1: "); Serial.print(P1); Serial.print("  "); Serial.print("VEL1: "); Serial.println(V1);
  Serial.print("ΔP2: "); Serial.print(P2); Serial.print("  "); Serial.print("VEL2: "); Serial.println(V2);
  Serial.print("ΔP3: "); Serial.print(P3); Serial.print("  "); Serial.print("VEL3: "); Serial.println(V3);

}
