#include "Parachute.h"

Parachute parachute(9); // 서보모터 9번 핀 사용

void setup() {
    parachute.begin();
}

void loop() {
    float yaw, pitch, roll;
    parachute.getSensorData(yaw, pitch, roll); // BNO055 센서 값 가져오기

    Serial.print("Yaw: ");
    Serial.print(yaw);
    Serial.print("°, Pitch: ");
    Serial.print(pitch);
    Serial.print("°, Roll: ");
    Serial.println(roll);

    parachute.update();
}
