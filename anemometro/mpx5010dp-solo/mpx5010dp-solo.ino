/*El código de este archivo tendrá como objetivo calibrar el sensor MPX5010DP (ajustar la función de transferencia).
  Este código SÓLAMENTE se puede probar en un ARDUINO.
*/

double Vout,P,Vs = 5.0;
double aux;
double tolP = 0.04; // Este valor se debe determinar
int i;

void setup()
{
  Serial.begin(9600);
}

void loop() 
{
   aux=0;
   for(i=0;i<10;i++){
    aux = aux + (float(analogRead(A0))*5.0/1023.0); //cálculo del voltaje
    delay(5);
   }
  Vout=aux/10.0;

  P = ( Vout - 0.04*Vs ) / (0.09 * Vs) + tolP; //Valor de la diferencia de presión usando la función de transferencia.
}
