#include <Arduino.h>
#include <PanelSensor.h>
#include <MOTOR.h>
#include <PS2X_lib.h>
#define PS2_DAT 10
#define PS2_CMD 11
#define PS2_SEL 12
#define PS2_CLK 13
PS2X ps2x;
uint8_t error = 0;

#define IR_Left_2 26
#define IR_Left_1 27
#define IR_Right_1 28
#define IR_Right_2 29

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

// ปู่บอด
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

void display_Color_2()
{
    // panel.rawSensor();
    Serial.print(panel.getRaw(0));
    Serial.print("\t");
    Serial.print(panel.getRaw(1));
    Serial.print("\t");
    Serial.print(panel.getRaw(2));
    Serial.print("\t");
    Serial.print(panel.getRaw(3));
    Serial.print("\t");
    Serial.print(panel.getRaw(4));
    Serial.print("\t");
    Serial.println(panel.getRaw(5));
    if (panel.getRaw(4) < panel.getRaw(3) && panel.getRaw(4) < panel.getRaw(5))
    {
        digitalWrite(led_Blue, LOW);
        digitalWrite(led_Red, LOW);
        digitalWrite(led_Green, HIGH);
    }
    else if (panel.getRaw(3) < panel.getRaw(4) && panel.getRaw(3) < panel.getRaw(5))
    {
        digitalWrite(led_Blue, LOW);
        digitalWrite(led_Red, HIGH);
        digitalWrite(led_Green, LOW);
    }
    else if (panel.getRaw(5) < panel.getRaw(3) && panel.getRaw(5) < panel.getRaw(4))
    {
        digitalWrite(led_Blue, HIGH);
        digitalWrite(led_Red, LOW);
        digitalWrite(led_Green, LOW);
    }
    else
    {
        digitalWrite(led_Blue, LOW);
        digitalWrite(led_Red, LOW);
        digitalWrite(led_Green, LOW);
    }
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
        motor_Left.speed(255);
        motor_Right.speed(255);
        delay(100);
        motor_Left.stop();
        motor_Right.stop();

        for (int i = 0; i < 3; i++)
        {
            digitalWrite(led_Blue, HIGH);
            digitalWrite(led_Red, HIGH);
            digitalWrite(led_Green, HIGH);
            delay(1000);
            digitalWrite(led_Blue, LOW);
            digitalWrite(led_Red, LOW);
            digitalWrite(led_Green, LOW);
            delay(1000);
        }
        uint32_t start = millis();
        uint32_t time = 4 * 1e3;
        while (millis() - start < time)
        {
            display_Color_2();
        }
        state = 1;
    }
    if (IR_Left_2_state == 0 && IR_Left_1_state == 0 && IR_Right_1_state == 0 && IR_Right_2_state == 0)
    {
        motor_Left.speed(175);
        motor_Right.speed(175);
    }

    if (IR_Left_2_state == 1 && IR_Left_1_state == 1 && IR_Right_1_state == 1 && IR_Right_2_state == 0)
    {
        motor_Left.speed(-255);
        motor_Right.speed(255);
    }
    if (IR_Left_2_state == 0 && IR_Left_1_state == 1 && IR_Right_1_state == 1 && IR_Right_2_state == 1)
    {
        motor_Left.speed(255);
        motor_Right.speed(-255);
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
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, false, false);
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(led_Blue, HIGH);
        digitalWrite(led_Red, HIGH);
        digitalWrite(led_Green, HIGH);
        delay(1500);
        digitalWrite(led_Blue, LOW);
        digitalWrite(led_Red, LOW);
        digitalWrite(led_Green, LOW);
        delay(500);
    }
}

void loop()
{
    // display_Color_2();
    if (state == 0)
    {
        TrackLine();
    }
    if (state == 1)
    {
        display_Color_2();
        ps2x.read_gamepad(false, 0);
        if (ps2x.Button(PSB_PAD_UP))
        {
            motor_Left.speed(255);
            motor_Right.speed(255);
        }
        else if (ps2x.Button(PSB_PAD_DOWN))
        {
            motor_Left.speed(-255);
            motor_Right.speed(-255);
        }
        else if (ps2x.Button(PSB_PAD_LEFT))
        {
            motor_Left.speed(-255);
            motor_Right.speed(255);
        }
        else if (ps2x.Button(PSB_PAD_RIGHT))
        {
            motor_Left.speed(255);
            motor_Right.speed(-255);
        }
        else
        {
            motor_Right.speed(0);
            motor_Left.speed(0);
        }
        delay(100);
    }
}
