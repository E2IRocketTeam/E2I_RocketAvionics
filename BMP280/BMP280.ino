#include <Wire.h>
#include "BMP280.h"

// BMP280 객체 생성
BMP280 bmp280;

void setup() {
    Serial.begin(9600);
    Wire.begin();

    // BMP280 초기화
    if (!bmp280.begin()) {
        Serial.println("BMP280 Initialization failed!");
        while (1);
    }
    Serial.println("BMP280 Initialized!");
}

void loop() {
    // BMP280 데이터 읽기
    float temperature = bmp280.readTemperature();
    float pressure = bmp280.readPressure();
    float altitude = bmp280.readAltitude(1013.25); // 해수면 기준 기압 (hPa)

    // 출력
    Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" °C");
    Serial.print("Pressure: "); Serial.print(pressure / 100); Serial.println(" hPa");
    Serial.print("Altitude: "); Serial.print(altitude); Serial.println(" m");
}
