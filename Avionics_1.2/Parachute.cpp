#include "Parachute.h"
#include <Arduino.h>

Parachute::Parachute(int servoPin) : sensor(0x28), servoPin(servoPin) {}

void Parachute::begin() {
    Serial.begin(9600);
    if (!sensor.begin()) {
        Serial.println("BNO055 연결 실패!");
        while (1);
    }

    sensor.setExternalCrystalUse(true);

    // 서보모터 초기화
    servoMotor.attach(servoPin);
    servoMotor.write(0); // 초기 위치 (0도)
}

void Parachute::update() {
    float yaw, pitch, roll;
    sensor.getEulerAngles(yaw, pitch, roll);

    // 서보모터 동작 조건
    if (abs(pitch - 90) >= thresholdAngle) {
        
        servoMotor.write(90);
    } else {
        
        servoMotor.write(0);
    }
}

// BNO055 센서 값을 외부에서 가져올 수 있도록 추가
void Parachute::getSensorData(float &yaw, float &pitch, float &roll) {
    sensor.getEulerAngles(yaw, pitch, roll);
}
