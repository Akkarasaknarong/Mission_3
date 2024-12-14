#include <Arduino.h>
#include <PanelSensor.h>
#include <MOTOR.h>
#include <PS2X_lib.h>

#define IR_Left_2 8
#define IR_Left_1 9
#define IR_Right_1 10
#define IR_Right_2 11

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

    if (IR_Left_2_state == 0 && IR_Left_1_state == 0 && IR_Right_1_state == 0 && IR_Right_2_state == 0)
    {
        Error = 0;
    }
    if (IR_Left_2_state == 0 && IR_Left_1_state == 1 && IR_Right_1_state == 0 && IR_Right_2_state == 0)
    {
        Error = 4.7;
    }
    if (IR_Left_2_state == 1 && IR_Left_1_state == 1 && IR_Right_1_state == 0 && IR_Right_2_state == 0)
    {
        Error = 4.9;
    }
    if (IR_Left_2_state == 1 && IR_Left_1_state == 0 && IR_Right_1_state == 0 && IR_Right_2_state == 0)
    {
        Error = 5.1;
    }
    if (IR_Left_2_state == 1 && IR_Left_1_state == 1 && IR_Right_1_state == 1 && IR_Right_2_state == 0)
    {
        Error = 5.1;
    }



    if (IR_Left_2_state == 0 && IR_Left_1_state == 0 && IR_Right_1_state == 1 && IR_Right_2_state == 0)
    {
        Error = -4.7;
    }
    if (IR_Left_2_state == 0 && IR_Left_1_state == 0 && IR_Right_1_state == 1 && IR_Right_2_state == 1)
    {
        Error = -4.9;
    }
    if (IR_Left_2_state == 0 && IR_Left_1_state == 0 && IR_Right_1_state == 0 && IR_Right_2_state == 1)
    {
        Error = -5.1;
    }
    if (IR_Left_2_state == 0 && IR_Left_1_state == 1 && IR_Right_1_state == 1 && IR_Right_2_state == 1)
    {
        Error = -5.1;
    }
    if (IR_Left_2_state == 1 && IR_Left_1_state == 1 && IR_Right_1_state == 1 && IR_Right_2_state == 1)
    {
        Error = 99;
    }

    if (Error == 0)
    {
        Speed_left = 200;
        Speed_right = 200;
        motor_Left.speed(Speed_left);
        motor_Right.speed(Speed_right);
    }
    else if (Error == 99)
    {
        motor_Left.speed(0);
        motor_Right.speed(0);
    }
    else
    {
        Speed_left = 50 * (Error * -1);
        Speed_right = 50 * Error;

        int minSpeed = -210;
        if (Error != 5.1 || Error != -5.1)
        {
            if (Speed_left < minSpeed)
            {
                Speed_left = minSpeed;
            }

            if (Speed_right < minSpeed)
            {
                Speed_right = minSpeed;
            }
        }

        motor_Left.speed(Speed_left);
        motor_Right.speed(Speed_right);
    }

    Serial.print(Speed_left);
    Serial.print("\t");
    Serial.println(Speed_right);
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
