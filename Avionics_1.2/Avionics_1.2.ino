#include <Wire.h>
#include "Sdcard.h"
#include "BMP390.h"
#include "BNO055.h"
#include "Parachute.h"
#include "LoRaSender.h"

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 920.9  // 주파수 920.9 MHz

Parachute parachute(9);

const char* filename = "sensor_data.csv";
LoRaSender LoRa(RFM95_CS, RFM95_RST, RFM95_INT, RF95_FREQ);
BMP390 bmpSensor;
BNO055 bnoSensor;

void setup() {
    Serial.begin(115200);
    while (!Serial);

    if (!initializeSD()) {
        Serial.println("SD card 모듈 초기화 실패");
        while (1);
    }

    if (!bmpSensor.begin()) {
        Serial.println("BMP390 모듈 초기화 실패");
        while (1);
    }

    if (!bnoSensor.begin()) {
        Serial.println("BNO055 모듈 초기화 실패");
        while (1);
    }

    if (!createLogFile(filename)) {
        Serial.println("Failed to create log file!");
        while (1);
    }

    if (!loRa.begin()) {
        Serial.println("LoRa 모듈 초기화 실패");
        while (1);
    }
    parachute.begin();
}

void loop() {
    float temperature, pressure, altitude;
    float yaw, pitch, roll;

    bmpSensor.readData(temperature, pressure, altitude);
    bnoSensor.readData(yaw, pitch, roll);
    parachute.update();
    
    if (logData(filename, yaw, pitch, roll, temperature, pressure, altitude)) {
        Serial.print("Logged Data: ");  // "Logged Data:" 뒤에 공백 추가
        Serial.print(yaw, 2); Serial.print(", ");
        Serial.print(pitch, 2); Serial.print(", ");
        Serial.print(roll, 2); Serial.print(", ");
        Serial.print(temperature, 2); Serial.print(", ");
        Serial.print(pressure, 2); Serial.print(", ");
        Serial.println(altitude, 2);  // 마지막은 Serial.println() 사용

    } else {
        Serial.println("Failed to log data");
    }
    LoRa.sendData(yaw, pitch, roll, temperature, pressure, altitude);
}
