#include <Wire.h>
#include <Adafruit_BMP3XX.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // I2C 통신 초기화
  if (!bmp.begin_I2C()) {  // 기본 I2C 주소는 0x77
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }

  // 센서 설정
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
}

void loop() {
  if (!bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  // 온도 출력
  Serial.print("Temperature = ");
  Serial.print(bmp.temperature);
  Serial.println(" *C");

  // 압력 출력
  Serial.print("Pressure = ");
  Serial.print(bmp.pressure / 100.0); // Pa -> hPa 변환
  Serial.println(" hPa");

  // 고도 출력
  Serial.print("Approx. Altitude = ");
  Serial.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.println();
}