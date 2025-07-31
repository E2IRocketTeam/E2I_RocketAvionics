#include "Parachute.h"
#include <Arduino.h>

Parachute::Parachute(int servoPin) : bno(0x4A), servoPin(servoPin), startTime(0) {}

void Parachute::begin() {
    // 센서 및 서보 초기화
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

    // 상태 변수 초기화
    maxAltitude = -9999;
    prevAltitude = 0;
    prevTime = 0;
    startTime = millis();
    deployed = false;
    initialized = false;
    
    // 클래스 멤버 변수 초기화
    this->yaw = 0; this->pitch = 0; this->roll = 0;
    this->temperature = 0; this->pressure = 0; this->altitude = 0;
    this->acceleration = {0, 0, 0};

    Serial.println("Parachute 시스템 초기화 완료!");
}

void Parachute::update() {
    // 센서 값 업데이트 및 저장
    bno.update();
    this->yaw = bno.getYaw();
    this->pitch = bno.getPitch();
    this->roll = bno.getRoll();
    this->acceleration = bno.getAccelerometer();
    
    bmp.readData(this->temperature, this->pressure, this->altitude);
    this->altitude = this->altitude - 110.0; // 고도 보정

    unsigned long currentTime = millis();
    float dt = (currentTime - prevTime) / 1000.0;

    if (!initialized || dt <= 0 || dt > 1.0) {
        prevAltitude = this->altitude;
        prevTime = currentTime;
        initialized = true;
        return;
    }

    float verticalSpeed = (this->altitude - prevAltitude) / dt;
    prevAltitude = this->altitude;
    prevTime = currentTime;

    // 15초 이후부터 사출 로직 및 최고 고도 갱신 시작
    if ((currentTime - startTime) > 5000) {
        
        // 1. 최고 고도 갱신 (사출 조건 판단을 위해 먼저 수행)
        if (this->altitude > maxAltitude) {
            maxAltitude = this->altitude;
        }

        // 2. 사출 조건 확인 (아직 사출되지 않았을 경우에만)
        if (!deployed) {
            // 조건 1: Z축 가속도가 음수로 바뀌면 즉시 사출
            if (this->acceleration.x < -2) {
                Serial.println("🚨 Z축 가속도 음수 감지! 낙하산 사출 🚨");
                servo.write(90);
                deployed = true;
                return;
            }

            // --- 요청하신 조건 추가 ---
            // 조건 2: 최고 고도에서 일정 높이 하강했거나, 하강 속도가 빠를 때 사출
            if ((this->altitude < maxAltitude - 0.5) && (verticalSpeed < -3.0 || abs(this->pitch - 90) > 45)) {
                Serial.println("🚨 낙하 감지 (정점 하강)! 낙하산 사출 🚨");
                servo.write(90);
                deployed = true;
                return;
            }
        }
    }
}

// --- Getter 함수 구현부 (기존과 동일) ---

void Parachute::getOrientation(float &yaw, float &pitch, float &roll) {
    yaw = this->yaw;
    pitch = this->pitch;
    roll = this->roll;
}

Vector3 Parachute::getAcceleration() const {
    return this->acceleration;
}

float Parachute::getAltitude() const {
    return this->altitude;
}

float Parachute::getPressure() const {
    return this->pressure;
}

float Parachute::getTemperature() const {
    return this->temperature;
}