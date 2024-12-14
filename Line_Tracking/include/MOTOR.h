
#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor{
    private:
        int pwmPin;
        int dirPin1;
        int dirPin2;
    public:
        Motor (int pwm ,int dir1, int dir2); 
        void speed(int speed); 
        void stop();                      
};

#endif 