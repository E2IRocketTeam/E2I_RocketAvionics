#ifndef PARACHUTE_H
#define PARACHUTE_H

#include <Servo.h>
#include "BNO085.h"
#include "BMP390.h"

class Parachute {
public:
    Parachute(int servoPin);
    void begin();
    void update();
    void getSensorData(float &yaw, float &pitch, float &roll);

private:
    BNO085 bno;
    BMP390 bmp;
    Servo servo;
    int servoPin;

    float maxAltitude = 0;
    float prevAltitude = 0;
    unsigned long prevTime = 0;
    unsigned long startTime = 0;
    bool deployed = false;
    bool initialized = false;
};

#endif
