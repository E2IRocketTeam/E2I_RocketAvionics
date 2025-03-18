#include <Adafruit_BNO08x.h>
#include <Wire.h>

// BNO085 객체 생성
Adafruit_BNO08x bno085;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  // I2C로 BNO085 초기화
  if (!bno085.begin_I2C()) {
    Serial.println("BNO085 초기화 실패! 센서를 확인하세요.");
    while (1);
  }
  Serial.println("BNO085 초기화 성공!");

  // 필요한 보고서를 활성화
  if (!bno085.enableReport(SH2_ACCELEROMETER)) {
    Serial.println("가속도 데이터 활성화 실패!");
  }
  if (!bno085.enableReport(SH2_GYROSCOPE_CALIBRATED)) {
    Serial.println("자이로 데이터 활성화 실패!");
  }
  if (!bno085.enableReport(SH2_MAGNETIC_FIELD_CALIBRATED)) {
    Serial.println("자력 데이터 활성화 실패!");
  }
}

void loop() {
  // 가속도 데이터
  sh2_SensorValue_t sensorValue;
  if (bno085.getSensorEvent(&sensorValue)) {
    if (sensorValue.sensorId == SH2_ACCELEROMETER) {
      Serial.print("Acc X: ");
      Serial.print(sensorValue.un.accelerometer.x, 2);
      Serial.print(", Y: ");
      Serial.print(sensorValue.un.accelerometer.y, 2);
      Serial.print(", Z: ");
      Serial.println(sensorValue.un.accelerometer.z, 2);
    }

    // 자이로 데이터
    if (sensorValue.sensorId == SH2_GYROSCOPE_CALIBRATED) {
      Serial.print("자이로 X: ");
      Serial.print(sensorValue.un.gyroscope.x, 2);
      Serial.print(", Y: ");
      Serial.print(sensorValue.un.gyroscope.y, 2);
      Serial.print(", Z: ");
      Serial.println(sensorValue.un.gyroscope.z, 2);
    }

    // 자력 데이터
    if (sensorValue.sensorId == SH2_MAGNETIC_FIELD_CALIBRATED) {
      Serial.print("자력 X: ");
      Serial.print(sensorValue.un.magneticField.x, 2);
      Serial.print(", Y: ");
      Serial.print(sensorValue.un.magneticField.y, 2);
      Serial.print(", Z: ");
      Serial.println(sensorValue.un.magneticField.z, 2);
    }
  }
}
