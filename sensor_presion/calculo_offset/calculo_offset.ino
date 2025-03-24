#include "ms4525do.h"

bfs::Ms4525do pres;
int numReadings = 50000;
float first_offset;
float sum;

void setup() {
  Serial.begin(9600);
  while(!Serial){}
  Wire.begin();
  Wire.setClock(400000);
  pres.Config(&Wire, 0x28, 1.0f, -1.0f);
  /* Starting communication with the pressure transducer */
  if (!pres.Begin()) {
    Serial.println("Error communicating with sensor");
    while(1){}
  }

  for (int i = 0; i < numReadings; i++) {
      sum += pres.pres_pa();
      Serial.println(sum);
  }
  first_offset = sum / numReadings; // Calcular el promedio como offset
  Serial.print("Offset calculado: ");
  Serial.println(first_offset);
}

void loop() {
  /* Read the sensor */
  if (pres.Read()) {
    Serial.print(pres.pres_pa(), 6); Serial.print("\t"); Serial.println(pres.pres_pa()-first_offset, 6);
  }
}
