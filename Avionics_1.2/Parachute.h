#ifndef PARACHUTE_H
#define PARACHUTE_H

#include "BNO055.h"
#include <Servo.h> // 서보모터 라이브러리 추가

class Parachute {
private:
    BNO055 sensor;   // BNO055 센서 객체
    Servo servoMotor; // 서보모터 객체
    int servoPin = 9;    // 서보모터 핀
    const int thresholdAngle = 90; // 서보모터 작동 임계각

public:
    Parachute();
    void begin();
    void update();
    void getSensorData(float &yaw, float &pitch, float &roll);
};

#endif
