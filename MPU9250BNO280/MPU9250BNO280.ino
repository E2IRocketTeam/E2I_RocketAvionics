#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <OneWire.h>
#include "MPU9255.h"

int16_t accelCount[3];
int16_t gyroCount[3];
int16_t magCount[3];
int16_t tempCount;
float f_accelCount[3];
float f_gyroCount[3];
float f_magCount[3];
float AccelScale, GyroScale, MagScale;
float temperature;
int doklG = 250; // stopni / sekunde - dokladnosc dla czujnika zyroskopowego
int doklA = 2; // +- 2g - dokladnosc dla akcelerometru
int doklM = 6; // 0.6 mGaussa - dokladnosc dla magnetometru
float approxGndPress;

/*
stworzenie obiektow czujnikow wchodzacych w sklad Waveshare 10 DOF IMU
*/
Adafruit_BMP280 bmp; // inicjalizacja czujnika cisnienia/altymetru + termometru
MPU9255 mpu(12, doklG, doklA, doklM); // inicjalizacja akcelerometru-zyroskopu-magnetometru

void setup() {
  Wire.begin(); // I2C 초기화
  TWBR = 24;
  Serial.begin(9600);

  // MPU9255 초기화
  mpu.initMPU9250();
  float magCalibration[3];
  mpu.initAK8963(magCalibration);

  // BMP280 초기화
  if (!bmp.begin(0x76)) {  // 주소 0x76으로 BMP280 초기화 (0x77일 경우 변경)
    Serial.println("BMP280 센서와 연결할 수 없습니다!");
    while (1);  // 오류 발생 시 무한 루프
  }

  // 압력 평균 계산
  for (int i = 0; i < 5; i++) {
    approxGndPress += bmp.readPressure();
    delay(1000);
  }
  approxGndPress /= 5;

  // GyroScale 설정
  switch (doklG) {
    case 250: GyroScale = 131.0; break;
    case 500: GyroScale = 65.5; break;
    case 1000: GyroScale = 32.8; break;
    case 2000: GyroScale = 16.4; break;
    default: GyroScale = 131.0; break;
  }

  // AccelScale 설정
  switch (doklA) {
    case 2: AccelScale = 16384.0; break;
    case 4: AccelScale = 8192.0; break;
    case 8: AccelScale = 4096.0; break;
    case 16: AccelScale = 2048.0; break;
    default: AccelScale = 16384.0; break;
  }

  // MagScale 설정
  switch (doklM) {
    case 6: MagScale = 0.6; break;
    case 15: MagScale = 0.15; break;
    default: MagScale = 1; break;
  }
}

void loop() {
  // 데이터 읽기 및 출력
  mpu.readAccelData(f_accelCount);
  for (int i = 0; i < 3; i++) f_accelCount[i] /= AccelScale; // 가속도 데이터를 스케일링

  Serial.print("\t |a| = "); 
  Serial.print(sqrt(sq(f_accelCount[0]) + sq(f_accelCount[1]) + sq(f_accelCount[2])));

  mpu.readGyroData(f_gyroCount);
  for (int i = 0; i < 3; i++) f_gyroCount[i] /= GyroScale; // 자이로 데이터를 스케일링

  Serial.print("\t |w| = "); 
  Serial.print(sqrt(sq(f_gyroCount[0]) + sq(f_gyroCount[1]) + sq(f_gyroCount[2])));

  mpu.readMagData(f_magCount);
  for (int i = 0; i < 3; i++) f_magCount[i] /= MagScale; // 자기장 데이터를 스케일링

  float kierunek = atan2(f_magCount[1], f_magCount[0]); // 자기장 방향 계산
  if (kierunek < 0) kierunek += 2 * PI;
  float kierStopnie = kierunek * 180 / PI; // 라디안을 각도로 변환
  Serial.print("\t kierunek = ");
  Serial.print(kierStopnie);

  // 온도 및 기압 출력
  tempCount = mpu.readTempData();
  Serial.print("\tT ");
  Serial.print(tempCount / 100.0); // 온도 출력
  Serial.print(" *C");

  Serial.print("\tP = ");
  Serial.print(bmp.readPressure() / 100.0); // 기압 출력
  Serial.print(" hPa");

  Serial.print("\tApprox alt = ");
  Serial.print(bmp.readAltitude(1019.0)); // 고도 출력 (표준 대기압 1019.0 hPa 기준)
  Serial.println(" m");

  // 1초 대기
}
