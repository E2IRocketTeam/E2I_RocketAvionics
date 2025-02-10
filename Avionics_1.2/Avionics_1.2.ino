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
//LoRaSender LoRa(RFM95_CS, RFM95_RST, RFM95_INT, RF95_FREQ);
BMP390 bmpSensor;
BNO055 bnoSensor;

void setup() {
    delay(100);  // SD 카드 안정성을 위해 초기 지연 추가

    if (!initializeSD()) {
        while (1);  // SD 카드 초기화 실패 시 멈춤
    }

    if (!bmpSensor.begin()) {
        while (1);  // BMP390 센서 초기화 실패 시 멈춤
    }

    if (!bnoSensor.begin()) {
        while (1);  // BNO055 센서 초기화 실패 시 멈춤
    }

    if (!createLogFile(filename)) {
        while (1);  // 로그 파일 생성 실패 시 멈춤
    }

    // if (!LoRa.begin()) {
    //     while (1);  // LoRa 모듈 초기화 실패 시 멈춤
    // }

    parachute.begin();
}

void loop() {
    float temperature, pressure, altitude;
    float yaw, pitch, roll;

    bmpSensor.readData(temperature, pressure, altitude);
    bnoSensor.readData(yaw, pitch, roll);
    parachute.update();
    
    logData(filename, yaw, pitch, roll, temperature, pressure, altitude);
    //LoRa.sendData(yaw, pitch, roll, temperature, pressure, altitude);
}
