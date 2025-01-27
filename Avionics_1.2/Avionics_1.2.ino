#include <Wire.h>
#include "BNO055.h"
#include "BMP280.h"
#include "SDCARD.h"

const char* logFilename = "sensor_log.csv"; // 저장할 파일 이름

// 센서 객체 생성
BNO055 bno055;
BMP280 bmp280;

void setup() {
    // Serial 초기화
    Serial.begin(9600);
    while (!Serial) {
        delay(10); // Serial 준비 대기
    }

    Serial.println("시작: 센서 데이터 SD 카드에 저장");

    // SD 카드 초기화
    if (!initializeSD()) {
        Serial.println("SD 카드 초기화 실패!");
        while (1);
    }
    Serial.println("SD 카드 초기화 성공!");

    // 데이터 파일 생성 및 헤더 작성
    if (!createLogFile(logFilename)) {
        Serial.println("데이터 파일 생성 실패!");
        while (1);
    }
    Serial.println("데이터 파일 생성 성공!");

    // BNO055 초기화
    if (!bno055.begin()) {
        Serial.println("BNO055 초기화 실패!");
        while (1);
    }
    bno055.setExternalCrystalUse(true);
    Serial.println("BNO055 초기화 완료!");

    // BMP280 초기화
    if (!bmp280.begin()) {
        Serial.println("BMP280 초기화 실패!");
        while (1);
    }
    Serial.println("BMP280 초기화 완료!");
}

void loop() {
    // BNO055 데이터
    float yaw, pitch, roll;
    bno055.getEulerAngles(yaw, pitch, roll);

    // BMP280 데이터
    float temperature = bmp280.readTemperature();
    float pressure = bmp280.readPressure() / 100.0; // Pa -> hPa 변환
    float altitude = bmp280.readAltitude(1013.25); // 해수면 기준 기압 (hPa)

    // 데이터 출력
    Serial.print("Yaw: "); Serial.print(yaw, 2);
    Serial.print(", Pitch: "); Serial.print(pitch, 2);
    Serial.print(", Roll: "); Serial.print(roll, 2);
    Serial.print(", Temp: "); Serial.print(temperature, 2);
    Serial.print(" °C, Pressure: "); Serial.print(pressure, 2);
    Serial.print(" hPa, Altitude: "); Serial.print(altitude, 2);
    Serial.println(" m");

    // 데이터 SD 카드에 저장
    if (!logData(logFilename, yaw, pitch, roll, temperature, pressure, altitude)) {
        Serial.println("데이터 저장 실패!");
    }
}
