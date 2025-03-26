#include <Arduino.h>
#include "BNO085.h"

// I2C 주소 0x4A로 설정 + 객체 이름은 bnoSensor 유지
BNO085 bnoSensor(0x4A);

void setup() {
    Wire.begin();  // Teensy 4.1 기본 I2C 초기화 (SDA = 18, SCL = 19)
    Serial.begin(115200);
    while (!Serial);

    Serial.println("Initializing BNO085...");

    if (!bnoSensor.begin()) {
        Serial.println("❌ Failed to initialize BNO085 at 0x4A!");
        while (1);
    }

    Serial.println("✅ BNO085 initialized.");
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
