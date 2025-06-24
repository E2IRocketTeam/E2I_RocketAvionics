#ifndef BNO085_H
#define BNO085_H

#include <Wire.h>
#include <Adafruit_BNO08x.h>

struct Vector3 {
    float x;
    float y;
    float z;
};

class BNO085 {
public:
    BNO085(uint8_t address = 0x4A);
    bool begin();
    void update();
    Vector3 getAccelerometer() const;
    Vector3 getGyroscope() const;
    Vector3 getMagnetometer() const;
    void readData(float &yaw, float &pitch, float &roll);

private:
    Adafruit_BNO08x bno;
    uint8_t address;
    Vector3 accelerometer;
    Vector3 gyroscope;
    Vector3 magnetometer;
    Vector3 rotation;
};

#endif
