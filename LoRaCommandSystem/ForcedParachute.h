#ifndef FORCED_PARACHUTE_H
#define FORCED_PARACHUTE_H

#include <Arduino.h>
#include <Servo.h>

class ForcedParachute {
public:
    ForcedParachute(); // 
    
    void begin();
    void openParachute();  // 낙하산 개방
    void closeParachute(); // 낙하산 닫기

private:
    Servo servoMotor;
    const int servoPin = 9; 
};

#endif
