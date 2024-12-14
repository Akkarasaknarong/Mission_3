#include <Arduino.h>
#include <PanelSensor.h>
#include <MOTOR.h>
#include <PS2X_lib.h>

#define IR_Left 8
#define IR_Right 9
#define led_Blue 49
#define led_Red 51
#define led_Green 53

Motor motor_Right(2, 4, 3);
Motor motor_Left(7, 6, 5);
int state = 0;

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
    Serial.print("Left : ");
    Serial.print(digitalRead(IR_Left));
    Serial.print("\t");
    Serial.print("Right : ");
    Serial.println(digitalRead(IR_Right));
}

void start()
{
    if (state == 0)
    {
        int ir_Left_state = digitalRead(IR_Left);
        int ir_Right_state = digitalRead(IR_Right);
        if (ir_Left_state == 0 && ir_Right_state == 0)
        {
            motor_Left.speed(173);
            motor_Right.speed(167);
        }
        else if (ir_Left_state == 0 && ir_Right_state == 1)
        {
            motor_Left.speed(255);
            motor_Right.speed(-255);
        }
        else if (ir_Left_state == 1 && ir_Right_state == 0)
        {
            motor_Left.speed(-255);
            motor_Right.speed(255);
        }
        else
        {
            motor_Left.speed(173);
            motor_Right.speed(167);
            delay(500);
            motor_Left.stop();
            motor_Right.stop();
            while (true)
            {
                motor_Left.stop();
                motor_Right.stop();
                int Color = panel.getColor();
                Serial.println(Color);
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
                state = 1;
            }
        }
    }
}

void setup()
{
    pinMode(led_Blue, OUTPUT);
    pinMode(led_Red, OUTPUT);
    pinMode(led_Green, OUTPUT);

    pinMode(IR_Left, INPUT);
    pinMode(IR_Right, INPUT);
    Serial.begin(115200);
}

void loop()
{
    display_Color();
    delay(80);
}
