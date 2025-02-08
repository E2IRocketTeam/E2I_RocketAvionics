#include <Wire.h>
#include "Sdcard.h"
#include "BMP390.h"
#include "BNO055.h"
#include <Servo.h>


const char* filename = "sensor_data.csv";
BMP390 bmpSensor;
BNO055 bnoSensor;

void setup() {
    Serial.begin(115200);
    while (!Serial);

    if (!initializeSD()) {
        Serial.println("SD card initialization failed!");
        while (1);
    }

    if (!bmpSensor.begin()) {
        Serial.println("BMP390 initialization failed!");
        while (1);
    }

    if (!bnoSensor.begin()) {
        Serial.println("BNO055 initialization failed!");
        while (1);
    }

    if (!createLogFile(filename)) {
        Serial.println("Failed to create log file!");
        while (1);
    }
    parachuteServo.attach(9); // 서보 모터 핀 (PWM 핀 사용)
    parachuteServo.write(0);  // 초기 위치 (낙하산 닫힘)
}

void loop() {
    float temperature, pressure, altitude;
    float yaw, pitch, roll;

    bmpSensor.readData(temperature, pressure, altitude);
    bnoSensor.readData(yaw, pitch, roll);

    if(pitch >= 45 && pitch <= 50) {
        Serial.println("🚀 낙하산 전개!");
        parachuteServo.write(90); // 90도 회전 (서보 모터가 낙하산을 전개하도록 설정)
    }


    if (logData(filename, yaw, pitch, roll, temperature, pressure, altitude)) {
        Serial.print(yaw, 2); Serial.print(", ");
        Serial.print(pitch, 2); Serial.print(", ");
        Serial.print(roll, 2); Serial.print(", ");
        Serial.print(temperature, 2); Serial.print(", ");
        Serial.print(pressure, 2); Serial.print(", ");
        Serial.println(altitude, 2);
    } else {
        Serial.println("Failed to log data");
    }
}
