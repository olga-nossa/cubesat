/*El código de este archivo tendrá como objetivo calibrar el sensor MPX5010DP (ajustar la función de transferencia).
  Este código SÓLAMENTE se puede probar en un ARDUINO.
*/

double Vout,P,Vs = 5.0;
double aux;
double tolP = 0; //0.04; // Este valor se debe determinar
int i;

void setup()
{
  Serial.begin(9600);
}

void loop() 
{
    Vout= (float(analogRead(A0))*5.0/1023.0);

  P = ((Vout - 0.04 * Vs) / (0.09 * Vs)) + tolP; //Valor de la diferencia de presión usando la función de transferencia [Pa].
  Serial.print("ADC = "); Serial.print(float(analogRead(A0))); Serial.print(" V = "); Serial.print(Vout); Serial.print(" P = "); Serial.println(P);
}
