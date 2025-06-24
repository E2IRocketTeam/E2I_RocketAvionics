#include <Arduino.h>
#include "BNO085.h"

BNO085 bnoSensor(0x4A);  // Teensy 4.1 기본 I2C 주소

void setup() {
    Wire.begin();  // Teensy 기본 I2C (SDA=18, SCL=19)
    Serial.begin(115200);
    while (!Serial);

    Serial.println("Initializing BNO085...");

    if (!bnoSensor.begin()) {
        Serial.println("Failed to initialize BNO085 at 0x4A");
        while (1);
    }

    Serial.println("BNO085 initialized.");
}

void loop() {
    bnoSensor.update();

    float yaw, pitch, roll;
    bnoSensor.readData(yaw, pitch, roll);

    Serial.print("Yaw: "); Serial.print(yaw);
    Serial.print(", Pitch: "); Serial.print(pitch);
    Serial.print(", Roll: "); Serial.println(roll);
}
