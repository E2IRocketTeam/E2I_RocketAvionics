#include "Parachute.h"
#include <Arduino.h>

Parachute::Parachute(int servoPin) : sensor(0x28), servoPin(servoPin) {}

void Parachute::begin() {
    Serial.begin(9600);
    if (!sensor.begin()) {
        Serial.println("BNO085 연결 실패!");
        while (1);
    }

    sensor.setExternalCrystalUse(true);

    // 서보모터 초기화
    servoMotor.attach(servoPin);
    servoMotor.write(0); // 초기 위치 (0도)
    Serial.println("서보모터 & BNO085 초기화 완료!");
}

void Parachute::update() {
    float yaw, pitch, roll;
    sensor.getEulerAngles(yaw, pitch, roll);
    
    Serial.print("Yaw: ");
    Serial.print(yaw);
    Serial.print("°, Pitch: ");
    Serial.print(pitch);
    Serial.print("°, Roll: ");
    Serial.println(roll);

    // 서보모터 동작 조건
    if (abs(pitch - 90) >= thresholdAngle) {
        Serial.println("🚨 ±45도 이상 감지! 서보모터 90도 이동 🚨");
        servoMotor.write(90);
    } else {
        Serial.println("✅ 정상 범위, 서보모터 0도 유지 ✅");
        servoMotor.write(0);
    }

    delay(100);
}

// BNO055 센서 값을 외부에서 가져올 수 있도록 추가
void Parachute::getSensorData(float &yaw, float &pitch, float &roll) {
    sensor.getEulerAngles(yaw, pitch, roll);
}
