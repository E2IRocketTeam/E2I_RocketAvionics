#include <Wire.h>
#include "BNO055.h"

// BNO055 객체 생성
BNO055 bno055;

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        delay(10); // Serial 준비 대기
    }

    Serial.println("BNO055 데이터 출력 시작!");

    // BNO055 초기화
    if (!bno055.begin()) {
        Serial.println("BNO055 초기화 실패... 확인 필요!");
        while (1);
    }

    delay(1000);

    // 센서 보정 정보 설정
    bno055.setExternalCrystalUse(true);
    Serial.println("BNO055 초기화 완료");
}

void loop() {
    // Euler 각도(Yaw, Pitch, Roll) 읽기
    float yaw, pitch, roll;
    bno055.getEulerAngles(yaw, pitch, roll);

    // Serial 출력
    Serial.print(yaw, 2); // Yaw
    Serial.print(",");
    Serial.print(pitch, 2); // Pitch
    Serial.print(",");
    Serial.println(roll, 2); // Roll

    // 딜레이
}
