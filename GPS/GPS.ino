#include "GPS.h"

// Teensy 4.1 Serial2 = RX2(8), TX2(7)
GPS gps(Serial2);

void setup() {
  Serial.begin(115200);
  gps.begin();  // 기본 9600bps

  Serial.println("=== GPS 테스트 시작 ===");
}

void loop() {
  gps.update();

  if (gps.isLocationUpdated()) {
    Serial.println("✅ GPS 데이터 수신 중...");
    Serial.print("위도: "); Serial.println(gps.getLatitude(), 6);
    Serial.print("경도: "); Serial.println(gps.getLongitude(), 6);
    Serial.print("고도: "); Serial.print(gps.getAltitude()); Serial.println(" m");
    Serial.print("위성 수: "); Serial.println(gps.getSatellites());
    Serial.print("UTC 날짜: "); Serial.println(gps.getDate());
    Serial.print("UTC 시간: "); Serial.println(gps.getTime());
    Serial.println("------------------------------");
    delay(1000);  // 1초 간격 출력
  }
}
