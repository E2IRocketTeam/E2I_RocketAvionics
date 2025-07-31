// ✅ Parachute 통합 클래스 (센서 + 낙하 조건 + 사출 제어 포함)

#include "Parachute.h"
#include <Arduino.h>

Parachute::Parachute(int servoPin) : bno(0x4A), servoPin(servoPin), startTime(0) {}

void Parachute::begin() {
    if (!bno.begin()) {
        Serial.println("BNO085 초기화 실패!");
        while (1);
    }
    if (!bmp.begin()) {
        Serial.println("BMP390 초기화 실패!");
        while (1);
    }
    servo.attach(servoPin);
    servo.write(0);

    maxAltitude = -9999; // 비정상 초기 고도 방지
    prevAltitude = 0;
    prevTime = 0;
    startTime = millis();
    deployed = false;
    initialized = false;

    Serial.println("Parachute 시스템 초기화 완료!");
}

void Parachute::update() {
    float yaw = 0, pitch = 0, roll = 0;
    bno.readData(yaw, pitch, roll);

    float temperature = 0, pressure = 0, altitude = 0;
    bmp.readData(temperature, pressure, altitude);
    altitude = altitude - 110.0; // 고도 보정 (실제 고도 기준 약 90m로 맞춤)

    unsigned long currentTime = millis();
    float dt = (currentTime - prevTime) / 1000.0;

    // 문제 1: 첫 실행 시 속도 계산 방지
    if (!initialized || dt <= 0 || dt > 1.0) {
        prevAltitude = altitude;
        prevTime = currentTime;
        initialized = true;
        Serial.println("[INFO] 첫 프레임 또는 비정상 dt로 속도 계산 생략");
        return;
    }

    float verticalSpeed = (altitude - prevAltitude) / dt;

    prevAltitude = altitude;
    prevTime = currentTime;

    // 15초 이내는 낙하 조건 및 maxAltitude 갱신 생략
    if ((currentTime - startTime) < 15000) {
        Serial.print("[INFO] 초기 15초 대기 중. 고도: ");
        Serial.print(altitude);
        Serial.print(" m, 속도: ");
        Serial.print(verticalSpeed);
        Serial.print(" m/s, Pitch: ");
        Serial.println(pitch);
        return;
    }

    // BNO085에서 Z축 가속도 확인
    Vector3 accel = bno.getAccelerometer();
    if (Serial.print(accel.z);
        !deployed && accel.z < 0) {
        Serial.println("🚨 Z축 가속도 음수 감지! 낙하산 사출 🚨");
        servo.write(90);
        deployed = true;
        return;
    }

    // 15초 이후부터 최고 고도 갱신
    if (altitude > maxAltitude)
        maxAltitude = altitude;

    Serial.print("고도: "); Serial.print(altitude);
    Serial.print(" m, 속도: "); Serial.print(verticalSpeed);
    Serial.print(" m/s, Pitch: "); Serial.println(pitch);

    /*if (!deployed &&
        (altitude < maxAltitude - 0.5) &&
        (verticalSpeed < -3.0 || abs(pitch - 90) > 45)) {
        Serial.println("🚨 낙하 감지! 낙하산 사출 🚨");
        servo.write(90);
        deployed = true; 
    }*/
}

void Parachute::getSensorData(float &yaw, float &pitch, float &roll) {
    bno.readData(yaw, pitch, roll);
}
