#ifndef BNO055_H
#define BNO055_H

#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>

class BNO055 {
public:
    BNO055();
    bool begin();
    void readData(float &yaw, float &pitch, float &roll);

private:
    Adafruit_BNO055 bno;
};

#endif
