#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

void setup(void)
{
  Serial.begin(9600);
  
  ads.setGain(GAIN_TWOTHIRDS);  // 2/3 ganancia - voltage de referencia: 6.144V - 1 bit = 0.1875mV

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
