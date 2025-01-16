#include <Wire.h>

// MPU9250 I2C 주소
#define MPU9250_ADDR 0x68

// MPU9250 레지스터 주소
#define PWR_MGMT_1 0x6B
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H 0x43

void setup() {
  Wire.begin();
  Serial.begin(9600);

  // MPU9250 초기화
  Wire.beginTransmission(MPU9250_ADDR);
  Wire.write(PWR_MGMT_1); // 전원 관리 레지스터
  Wire.write(0x00);       // 장치를 깨움
  Wire.endTransmission();
  delay(100);

  Serial.println("MPU9250 Initialized!");
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

  // 데이터 출력
  Serial.print("Accelerometer: ");
  Serial.print("X="); Serial.print(accel_x);
  Serial.print(" Y="); Serial.print(accel_y);
  Serial.print(" Z="); Serial.println(accel_z);

  Serial.print("Gyroscope: ");
  Serial.print("X="); Serial.print(gyro_x);
  Serial.print(" Y="); Serial.print(gyro_y);
  Serial.print(" Z="); Serial.println(gyro_z);

  Serial.println("-----------------------------");
}

// MPU9250 데이터 읽기 함수
int16_t readMPU9250(uint8_t reg) {
  Wire.beginTransmission(MPU9250_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false); // I2C 연결 유지
  Wire.requestFrom(MPU9250_ADDR, 2, true); // 2바이트 읽기

  // 읽은 데이터 병합 (MSB + LSB)
  int16_t data = Wire.read() << 8 | Wire.read();
  return data;
}
