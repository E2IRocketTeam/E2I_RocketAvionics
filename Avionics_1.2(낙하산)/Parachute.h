#ifndef PARACHUTE_H
#define PARACHUTE_H

#include <Servo.h>
#include "BNO085.h"
#include "BMP390.h"

class Parachute {
public: // 외부에서 접근 가능한 함수들
    Parachute(int servoPin);
    void begin();
    void update();

    // --- 메인 스케치에서 호출할 함수들을 여기에 선언합니다 ---
    void getOrientation(float &yaw, float &pitch, float &roll);
    Vector3 getAcceleration() const;
    float getAltitude() const;
    float getPressure() const;
    float getTemperature() const;

private: // 클래스 내부에서만 사용하는 변수들
    // 센서 객체
    BNO085 bno;
    BMP390 bmp;
    Servo servo;
    int servoPin;

    // 상태 관리 변수
    float maxAltitude;
    float prevAltitude;
    unsigned long prevTime;
    unsigned long startTime;
    bool deployed;
    bool initialized;

    // --- 센서 데이터 저장을 위한 멤버 변수들을 여기에 선언합니다 ---
    float yaw, pitch, roll;
    float temperature, pressure, altitude;
    Vector3 acceleration;
};

#endif