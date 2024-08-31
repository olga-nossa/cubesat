#include "ms4525do.h"
#define alpha 0.03
float pressure_filtered = 0;
float velocity_filtered = 0;
bfs::Ms4525do pres;

void apply_filter(float pressure) {
  pressure_filtered = alpha * pres.pres_pa() + (1 - alpha) * pressure_filtered;
  Serial.print(pressure_filtered, 6);
  Serial.print("/");

  if ((pressure_filtered + 90) < 0) {
    velocity_filtered = 0;
  } else {
    velocity_filtered = sqrt(2 * (pressure_filtered + 90) / 1.006);
  }
  Serial.println(velocity_filtered, 6);
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  Wire.begin();
  Wire.setClock(400000);
  pres.Config(&Wire, 0x28, 1.0f, -1.0f);

  if (!pres.Begin()) {
    Serial.println("Error communicating with sensor");
    while (1) {}
  }
}

void loop() {
  if (pres.Read()) {
    Serial.print(pres.pres_pa(), 6);
    Serial.print("/");
    float pressure = pres.pres_pa();
    apply_filter(pressure);
  }
}
