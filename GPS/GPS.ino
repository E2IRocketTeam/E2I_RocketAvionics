#include <TinyGPSPlus.h>

// TinyGPS 객체
TinyGPSPlus gps;

// Serial2 → RX = 7번, TX = 8번
#define gpsSerial Serial2

void setup() {
  Serial.begin(115200);      // PC 출력용
  gpsSerial.begin(9600);     // GPS 모듈 통신속도

  Serial.println("GPS 테스트 시작 (핀 7:RX, 8:TX)");
}

void loop() {
  // GPS 데이터 수신 및 파싱
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  // 위도/경도 정보 출력
  if (gps.location.isUpdated()) {
    Serial.print("위도: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(", 경도: ");
    Serial.println(gps.location.lng(), 6);
  }

  // 시간 정보 출력
  if (gps.time.isUpdated()) {
    Serial.print("시간: ");
    Serial.print(gps.time.hour());
    Serial.print(":");
    Serial.print(gps.time.minute());
    Serial.print(":");
    Serial.println(gps.time.second());
  }

  delay(200);  // 출력 간격
}
