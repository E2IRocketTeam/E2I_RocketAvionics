#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// MPU9250 I2C 주소
#define MPU9250_ADDR 0x68
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H 0x43
#define MAG_XOUT_L 0x03 // Magnetometer 주소 (AK8963)

// BMP280 객체 생성
Adafruit_BMP280 bmp; // BMP280 라이브러리 사용

// MPU9250 초기화
void initMPU9250() {
  Wire.beginTransmission(MPU9250_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1 레지스터
  Wire.write(0x00); // 장치 활성화
  Wire.endTransmission();
  delay(100);
}

// MPU9250 데이터 읽기 함수
int16_t readMPU9250(uint8_t reg) {
  Wire.beginTransmission(MPU9250_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU9250_ADDR, 2, true);

  int16_t data = Wire.read() << 8 | Wire.read();
  return data;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // MPU9250 초기화
  initMPU9250();
  Serial.println("MPU9250 Initialized!");

  // BMP280 초기화
  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 Initialization failed!");
    while (1);
  }
  Serial.println("BMP280 Initialized!");
}

void loop() {
  // 가속도 데이터 읽기
  int16_t accel_x = readMPU9250(ACCEL_XOUT_H);
  int16_t accel_y = readMPU9250(ACCEL_XOUT_H + 2);
  int16_t accel_z = readMPU9250(ACCEL_XOUT_H + 4);

  // 자이로 데이터 읽기
  int16_t gyro_x = readMPU9250(GYRO_XOUT_H);
  int16_t gyro_y = readMPU9250(GYRO_XOUT_H + 2);
  int16_t gyro_z = readMPU9250(GYRO_XOUT_H + 4);

  // BMP280 데이터 읽기
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure();
  float altitude = bmp.readAltitude(1013.25); // 해수면 기준 기압 (hPa)

  // 출력
  Serial.print("Accelerometer: X="); Serial.print(accel_x);
  Serial.print(" Y="); Serial.print(accel_y);
  Serial.print(" Z="); Serial.println(accel_z);

  Serial.print("Gyroscope: X="); Serial.print(gyro_x);
  Serial.print(" Y="); Serial.print(gyro_y);
  Serial.print(" Z="); Serial.println(gyro_z);

  Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" °C");
  Serial.print("Pressure: "); Serial.print(pressure / 100); Serial.println(" hPa");
  Serial.print("Altitude: "); Serial.print(altitude); Serial.println(" m");

  Serial.println("-----------------------------");
  delay(500);
}
