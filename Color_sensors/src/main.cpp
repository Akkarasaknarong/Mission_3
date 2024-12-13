#include <Arduino.h>
#include <PanelSensor.h>

#define NUM_SENSORS 6
#define SETPOINT 2500
uint8_t PINS[NUM_SENSORS] = {A0, A1, A2, A3, A4, A5};
PanelSensor panel(PINS, NUM_SENSORS);
uint16_t maxValue[NUM_SENSORS] = { 377, 529, 635, 651, 644, 342, };
uint16_t minValue[NUM_SENSORS] = { 45, 50, 56, 57, 53, 46, };

void setup() {
  Serial.begin(115200);
  panel.begin(maxValue, minValue);
}

void loop() {
  // panel.rawSensor();
  int color_signal = panel.getColor();
  Serial.println(color_signal);

}
