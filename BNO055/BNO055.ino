#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // Serial 준비 대기
  }

  Serial.println("BNO055 테스트 시작!");

  // 센서 초기화
  if (!bno.begin()) {
    Serial.println("BNO055 초기화 실패... 확인 필요!");
    while (1);
  }
  delay(1000);

  // 센서 보정 정보 출력
  bno.setExtCrystalUse(true);
  Serial.println("BNO055 초기화 완료");
}

void loop() {
  // Euler 각도(Yaw, Pitch, Roll) 읽기
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  Serial.print(euler.x(), 2); // Yaw
  Serial.print(",");
  Serial.print(euler.y(), 2); // Pitch
  Serial.print(",");
  Serial.println(euler.z(), 2); // Roll

  // 딜레이
  delay(10); // 10ms 주기 (100Hz)
}
