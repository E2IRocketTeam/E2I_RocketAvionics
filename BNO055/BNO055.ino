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

  if (!bno.begin()) {
    Serial.println("BNO055 초기화 실패... 확인 필요!");
    while (1);
  }
  delay(1000);

  bno.setExtCrystalUse(true);
  Serial.println("BNO055 초기화 완료");
}

void loop() {
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  int8_t temp = bno.getTemp();

  // 데이터를 CSV 형식으로 전송
  Serial.print(euler.x());
  Serial.print(",");
  Serial.print(euler.y());
  Serial.print(",");
  Serial.print(euler.z());
  Serial.print(",");
  Serial.println(temp);
}
