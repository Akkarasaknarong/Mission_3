#include "MOTOR.h"

Motor Motor_Left(2, 3, 4);
Motor Motor_Right(5, 6, 7);

#define ir_0 9
#define ir_1 8

int status = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ir_0, INPUT);
  pinMode(ir_1, INPUT);
}

void loop() {
  int ir0State = digitalRead(ir_0);
  int ir1State = digitalRead(ir_1);
  if (status == 0) {
    if (ir0State == 1 && ir1State == 1) {
      Motor_Left.stop();
      Motor_Right.stop();
      status = 1;
    } else if (ir0State == 0 && ir1State == 1) {
      Motor_Left.speed(255);
      Motor_Right.stop();
    } else if (ir0State == 1 && ir1State == 0) {
      Motor_Left.stop();
      Motor_Right.speed(255);
    } else if (ir0State == 0 && ir1State == 0) {
      Motor_Left.speed(255);
      Motor_Right.speed(255);
    }
  }
  // Serial.print("Status : ");
  // Serial.println(status);
  // Serial.print("CH1 : ");
  // Serial.println(ir0State);
  // Serial.print("CH2 : ");
  // Serial.println(ir1State);
  // Serial.println();
  // delay(500);
}
