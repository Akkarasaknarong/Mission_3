
#include "MOTOR.h"


Motor::Motor(int pwm, int dir1, int dir2) {
  pwmPin = pwm;
  dirPin1 = dir1;
  dirPin2 = dir2;

  pinMode(pwmPin, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);
}

void Motor::speed(int speed) {
  if (speed > 0) {
    digitalWrite(dirPin1, HIGH);
    digitalWrite(dirPin2, LOW);
  } else if (speed < 0) {
    digitalWrite(dirPin1, LOW);
    digitalWrite(dirPin2, HIGH);
  } else {
    analogWrite(pwmPin, 0);
  }
  analogWrite(pwmPin, abs(speed));
}

void Motor::stop() {
  analogWrite(pwmPin, 255);
  digitalWrite(dirPin1, HIGH);
  digitalWrite(dirPin2, HIGH);
}