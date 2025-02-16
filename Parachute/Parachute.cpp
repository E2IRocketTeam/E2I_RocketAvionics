#include "Parachute.h"
#include <Arduino.h>

// 기본 생성자 (서보핀은 9번으로 고정)
Parachute::Parachute() : sensor(0x28) {}

void Parachute::begin() {
    Serial.begin(9600);
    if (!sensor.begin()) {
        while (1);
    }

    sensor.setExternalCrystalUse(true);

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

    delay(100);
}

// BNO055 센서 값을 외부에서 가져올 수 있도록 추가
void Parachute::getSensorData(float &yaw, float &pitch, float &roll) {
    sensor.getEulerAngles(yaw, pitch, roll);
}
