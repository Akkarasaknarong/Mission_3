#include <Arduino.h>
#include <PanelSensor.h>
#include <MOTOR.h>
#include <PS2X_lib.h>

#define IR_Left_2 22
#define IR_Left_1 23
#define IR_Right_1 24
#define IR_Right_2 25

#define led_Blue 49
#define led_Red 51
#define led_Green 53

Motor motor_Right(2, 3, 4);
Motor motor_Left(7, 6, 5);
int state = 0;
float Error;
int Speed_left, Speed_right;

#define NUM_SENSORS 6
#define SETPOINT 2500
uint8_t PINS[NUM_SENSORS] = {A0, A1, A2, A3, A4, A5};
PanelSensor panel(PINS, NUM_SENSORS);

void display_Color()
{
    int Color = panel.getColor();
    // Serial.println(Color);
    if (Color == 1)
    {
        digitalWrite(led_Red, HIGH);
    }
    else if (Color == 2)
    {
        digitalWrite(led_Green, HIGH);
    }
    else if (Color == 3)
    {
        digitalWrite(led_Blue, HIGH);
    }
    else
    {
        digitalWrite(led_Blue, LOW);
        digitalWrite(led_Red, LOW);
        digitalWrite(led_Green, LOW);
    }
    panel.rawSensor();
}

void display_IR()
{
    Serial.print(digitalRead(IR_Left_2));
    Serial.print("\t");
    Serial.print(digitalRead(IR_Left_1));
    Serial.print("\t");
    Serial.print(digitalRead(IR_Right_1));
    Serial.print("\t");
    Serial.println(digitalRead(IR_Right_2));
}

void TrackLine()
{
    int IR_Left_2_state = digitalRead(IR_Left_2);
    int IR_Left_1_state = digitalRead(IR_Left_1);
    int IR_Right_1_state = digitalRead(IR_Right_1);
    int IR_Right_2_state = digitalRead(IR_Right_2);

    if (IR_Left_2_state == 1 && IR_Left_1_state == 1 && IR_Right_1_state == 1 && IR_Right_2_state == 1)
    {
        motor_Left.speed(0);
        motor_Right.speed(0);
    }
    if (IR_Left_2_state == 0 && IR_Left_1_state == 0 && IR_Right_1_state == 0 && IR_Right_2_state == 0)
    {
        motor_Left.speed(180);
        motor_Right.speed(180);
    }
    if (IR_Left_2_state == 0 && IR_Left_1_state == 1 && IR_Right_1_state == 0 && IR_Right_2_state == 0)
    {
        motor_Left.speed(-100);
        motor_Right.speed(230);
    }
    if (IR_Left_2_state == 1 && IR_Left_1_state == 1 && IR_Right_1_state == 0 && IR_Right_2_state == 0)
    {
        motor_Left.speed(-255);
        motor_Right.speed(255);
    }
    if (IR_Left_2_state == 1 && IR_Left_1_state == 0 && IR_Right_1_state == 0 && IR_Right_2_state == 0)
    {
        motor_Left.speed(-255);
        motor_Right.speed(255);
    }
    if (IR_Left_2_state == 1 && IR_Left_1_state == 1 && IR_Right_1_state == 1)
    {
        motor_Left.speed(0);
        motor_Right.speed(0);
    }

    if (IR_Left_2_state == 0 && IR_Left_1_state == 0 && IR_Right_1_state == 1 && IR_Right_2_state == 0)
    {
        motor_Right.speed(-100);
        motor_Left.speed(230);
    }
    if (IR_Left_2_state == 0 && IR_Left_1_state == 0 && IR_Right_1_state == 1 && IR_Right_2_state == 1)
    {
        motor_Right.speed(-255);
        motor_Left.speed(255);
    }
    if (IR_Left_2_state == 0 && IR_Left_1_state == 0 && IR_Right_1_state == 0 && IR_Right_2_state == 1)
    {
        motor_Right.speed(-255);
        motor_Left.speed(255);
    }
    if (IR_Left_1_state == 1 && IR_Right_1_state == 1 && IR_Right_2_state == 1)
    {
        motor_Right.speed(0);
        motor_Left.speed(0);
    }
    if (IR_Left_2_state == 1 && IR_Left_1_state == 1 && IR_Right_1_state == 1 && IR_Right_2_state == 1)
    {
        motor_Right.speed(-255);
        motor_Left.speed(255);
    }
}

void setup()
{
    pinMode(led_Blue, OUTPUT);
    pinMode(led_Red, OUTPUT);
    pinMode(led_Green, OUTPUT);

    pinMode(IR_Left_1, INPUT);
    pinMode(IR_Left_2, INPUT);
    pinMode(IR_Right_1, INPUT);
    pinMode(IR_Right_2, INPUT);
    Serial.begin(115200);
}

void loop()
{
    TrackLine();
}
