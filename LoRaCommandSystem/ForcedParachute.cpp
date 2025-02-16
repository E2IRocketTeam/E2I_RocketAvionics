#include "ForcedParachute.h"
#include <Arduino.h>

ForcedParachute::Parachute() : sensor(0x28) {}

void ForcedParachute::begin() {
    Serial.begin(9600);
    servoMotor.attach(servoPin);
    servoMotor.write(0); // 초기 상태 (닫힘)
}

// 낙하산 개방 (서보모터를 90도로 이동)
void ForcedParachute::openParachute() {
    servoMotor.write(90);
}

// 낙하산 닫기 (서보모터를 0도로 이동)
void ForcedParachute::closeParachute() {
    servoMotor.write(0);
}
