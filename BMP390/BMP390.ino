#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include "Kalman.h"

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;

Kalman kalmanTemp;
Kalman kalmanPressure;
Kalman kalmanAltitude;

unsigned long prevTime = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!bmp.begin_I2C()) {
    Serial.println("BMP390 센서를 찾을 수 없습니다. 연결을 확인해주세요.");
    while (1);
  }

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  bmp.performReading();
  float initialTemp = bmp.temperature;
  float initialPressure = bmp.pressure / 100.0;
  float initialAltitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);

  kalmanTemp.setAngle(initialTemp);
  kalmanPressure.setAngle(initialPressure);
  kalmanAltitude.setAngle(initialAltitude);

  prevTime = millis();

  // 헤더 출력
  Serial.println("RawTemp(°C), FilteredTemp(°C), RawPress(hPa), FilteredPress(hPa), RawAlt(m), FilteredAlt(m)");
}

void loop() {
  if (!bmp.performReading()) {
    Serial.println("센서 데이터 읽기 실패");
    return;
  }

  unsigned long currentTime = millis();
  float dt = (currentTime - prevTime) / 1000.0;
  prevTime = currentTime;

  float temp = bmp.temperature;
  float pressure = bmp.pressure / 100.0;
  float altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);

  float filteredTemp = kalmanTemp.getAngle(temp, 0.0, dt);
  float filteredPressure = kalmanPressure.getAngle(pressure, 0.0, dt);
  float filteredAltitude = kalmanAltitude.getAngle(altitude, 0.0, dt);

  // 한 줄에 원본값 + 필터값 출력
  Serial.print(temp, 2);
  Serial.print(", ");
  Serial.print(filteredTemp, 2);
  Serial.print(", ");
  Serial.print(pressure, 2);
  Serial.print(", ");
  Serial.print(filteredPressure, 2);
  Serial.print(", ");
  Serial.print(altitude, 2);
  Serial.print(", ");
  Serial.println(filteredAltitude, 2);

  delay(100);
}
