#ifndef BNO055_H
#define BNO055_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

class BNO055 {
public:
    BNO055(uint8_t address = 0x28); // I2C 주소를 받을 수 있도록 생성자 수정
    bool begin();
    void setExternalCrystalUse(bool use);
    void getEulerAngles(float &yaw, float &pitch, float &roll);

private:
    Adafruit_BNO055 bno;
    uint8_t address;
};

#endif
