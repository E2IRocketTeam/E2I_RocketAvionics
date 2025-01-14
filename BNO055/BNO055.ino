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
    Serial.print("BNO055 초기화 실패... 확인 필요!");
    while (1);
  }
  delay(1000);

  // 센서 보정 정보 출력
  bno.setExtCrystalUse(true);
  Serial.println("BNO055 초기화 완료");
}

void loop() {

imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

Serial.print("Yaw: ");
Serial.print(euler.x());
Serial.print(" Pitch: ");
Serial.print(euler.y());
Serial.print(" Roll: ");
Serial.println(euler.z());

  // 온도 출력
int8_t temp = bno.getTemp();
Serial.print("온도: ");
Serial.print(temp);
Serial.println(" °C");

  // 딜레이
delay(1000);
}
