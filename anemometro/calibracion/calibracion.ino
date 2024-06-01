#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads;

double P;
double V;
#define Vs 5.0 //Voltaje de alimentación de los sensores
#define rho 1.225 //Valor de la densidad del aire

void setup() {
  Serial.begin(9600);
  ads.setGain(GAIN_TWOTHIRDS);  // 2/3 ganancia - voltaje de referencia: 6.144V - 1 bit = 0.1875mV
    if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
}

void loop() {
  int16_t adc;
  float volts;

  adc = ads.readADC_SingleEnded(0); //cambiar entrada que está entre paréntesis
  volts = ads.computeVolts(adc);
  P = 1000 * (( volts - 0.04 * Vs ) / (0.09  * Vs)); //ajustar tolerancia
  V = sqrt(2 * P/rho);

  Serial.print("digital: "); Serial.print(adc);
  Serial.print(" voltaje: "); Serial.print(volts,6);
  Serial.print(" ΔP0: "); Serial.print(P,6);
  Serial.print(" velocidad "); Serial.println(V,6);
}
