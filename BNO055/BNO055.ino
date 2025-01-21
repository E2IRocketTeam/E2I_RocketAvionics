#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <SD.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

// SD 카드 칩 선택 핀 설정 (Teensy 4.1 내장 SD는 BUILTIN_SDCARD 사용)
const int chipSelect = BUILTIN_SDCARD;

File dataFile;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10); // Serial 준비 대기
  }

  Serial.println("BNO055 + SD 카드 데이터 로깅 시작!");

  // BNO055 초기화
  if (!bno.begin()) {
    Serial.println("BNO055 초기화 실패... 확인 필요!");
    while (1);
  }
  delay(1000);

  // 센서 보정 정보 설정
  bno.setExtCrystalUse(true);
  Serial.println("BNO055 초기화 완료");

  // SD 카드 초기화
  if (!SD.begin(chipSelect)) {
    Serial.println("SD 카드 초기화 실패!");
    while (1);
  }
  Serial.println("SD 카드 초기화 완료");

  // 데이터 파일 열기 (덮어쓰기 모드)
  dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Yaw,Pitch,Roll"); // CSV 헤더 작성
    dataFile.close();
  } else {
    Serial.println("파일 열기 실패!");
    while (1);
  }
}

void loop() {
  // Euler 각도(Yaw, Pitch, Roll) 읽기
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  float yaw = euler.x();
  float pitch = euler.y();
  float roll = euler.z();

  // Serial 출력
  Serial.print(euler.x(), 2); // Yaw
  Serial.print(",");
  Serial.print(euler.y(), 2); // Pitch
  Serial.print(",");
  Serial.println(euler.z(), 2); // Roll

  // SD 카드에 데이터 저장
  dataFile = SD.open("datalog.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.print(yaw, 2);
    dataFile.print(",");
    dataFile.print(pitch, 2);
    dataFile.print(",");
    dataFile.println(roll, 2);
    dataFile.close();
  } else {
    Serial.println("파일 열기 실패!");
  }

  // 딜레이
  delay(10); // 10ms 주기 (100Hz)
}
