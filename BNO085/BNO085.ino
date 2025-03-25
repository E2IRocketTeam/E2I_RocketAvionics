#include "BNO085.h"

BNO085 bnoSensor;

void setup() {
    Serial.begin(115200);
    while (!Serial);

    if (!bnoSensor.begin()) {
        Serial.println("센서 초기화 실패!");
        while (1);
    }
    Serial.println("센서 초기화 성공!");
}

void loop() {
    bnoSensor.update();

    float yaw, pitch, roll;
    bnoSensor.readData(yaw, pitch, roll);

    Serial.print("Yaw: "); Serial.print(yaw);
    Serial.print(", Pitch: "); Serial.print(pitch);
    Serial.print(", Roll: "); Serial.println(roll);

    delay(100);
}
