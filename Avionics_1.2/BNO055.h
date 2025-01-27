#ifndef BNO055_H
#define BNO055_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

class BNO055 {
public:
    BNO055(uint8_t address = 55); // 기본 I2C 주소는 55
    bool begin();
    void setExternalCrystalUse(bool use);
    void getEulerAngles(float &yaw, float &pitch, float &roll);

private:
    Adafruit_BNO055 bno;
    uint8_t address;
};

#endif
