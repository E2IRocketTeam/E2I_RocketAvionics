#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// BMP280 객체 생성
Adafruit_BMP280 bmp; // BMP280 라이브러리 사용

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // BMP280 초기화
  if (!bmp.begin(0x76)) { // BMP280의 I2C 주소 (0x76 또는 0x77 사용 가능)
    Serial.println("BMP280 Initialization failed!");
    while (1);
  }
  Serial.println("BMP280 Initialized!");
}

void loop() {
  // BMP280 데이터 읽기
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure();
  float altitude = bmp.readAltitude(1013.25); // 해수면 기준 기압 (hPa)

  // 출력
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" °C");
  Serial.print("Pressure: "); Serial.print(pressure / 100); Serial.println(" hPa");
  Serial.print("Altitude: "); Serial.print(altitude); Serial.println(" m");

  Serial.println("-----------------------------");
  delay(500);
}
