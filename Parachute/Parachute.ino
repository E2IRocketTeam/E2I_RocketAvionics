#include <Arduino.h>
#include "Parachute.h"

const int SERVO_PIN = 22;             // 서보모터 연결 핀
Parachute parachute(SERVO_PIN);     // 낙하산 객체 생성

void setup() {
    Serial.begin(9600);
    parachute.begin();             // BNO085 + BMP390 + 서보 초기화
}

void loop() {
    parachute.update();           // 낙하 조건 확인 및 사출 수행
    delay(100);                   // 주기적으로 10Hz (100ms)
}
