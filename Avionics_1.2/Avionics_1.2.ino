// E2I Rocket Avionics Main Control Code (v1.4 + GPS + 주석 추가)
#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>

#include "Parachute.h"  // 낙하산 및 센서 모듈 (BNO085, BMP390 포함)
#include "Sdcard.h"     // SD카드 로그 저장 함수
#include "BNO085.h"     // 자세 센서
#include "BMP390.h"     // 고도/기압/온도 센서
#include "GPS.h"        // GPS 모듈 클래스
#include "Kalman.h"     // 칼만 필터 (필요 시 사용)


// --- LoRa 핀 설정 및 주파수 ---
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 915.0  // 미국용 ISM 대역, 한국은 보통 920~923MHz 사용

// --- 서보 핀 (낙하산용) ---
const int SERVO_PIN = 22;

// --- 전역 객체 생성 ---
RH_RF95 rf95(RFM95_CS, RFM95_INT);         // LoRa 객체
Parachute parachute(SERVO_PIN);            // 낙하산 및 센서 통합 클래스
GPS gps(Serial1);                           // GPS 모듈 (Serial1 사용)

// --- SD 로그 파일 이름 및 시간 기록용 변수 ---
const char* logFilename = "flight_log.csv";
unsigned long programStartTime = 0; // 안정화 대기 시간 기준

void setup() {
  Serial.begin(115200);
  while (!Serial);  // USB 시리얼 연결 대기
  Serial.println("===== Avionics System Start (v1.4 + GPS) =====");

  // 1. 낙하산 시스템 초기화 (센서 포함)
  parachute.begin();

  // 2. SD 카드 초기화 및 로그 파일 생성
  if (!initializeSD()) {
    Serial.println("Error: SD card initialization failed!");
    while (1) delay(10);
  }
  if (!createLogFile(logFilename)) {
    Serial.println("Error: Failed to create log file!");
    while (1) delay(10);
  }

  // 3. LoRa 무선 통신 초기화
  if (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  rf95.setFrequency(RF95_FREQ);
  rf95.setTxPower(23, false);  // 최대 전송 출력

  // 4. GPS 초기화
  gps.begin(9600);
  Serial.println("GPS initialized");

  // 5. 안정화 대기 시간 기록
  programStartTime = millis();
  Serial.println("Sensor stabilization period started (5 seconds)...");
}

void loop() {
  // 1. 낙하산 시스템 업데이트 (센서값 내부 계산 포함)
  parachute.update();

  // 2. GPS NMEA 수신 처리
  gps.update();

  // 3. 안정화 시간 대기 중이면 . 출력하고 종료
  if (millis() - programStartTime < 5000) {
    Serial.print(".");
    delay(500);
    return;
  }

  // 4. 센서 데이터 수집
  float yaw, pitch, roll;
  float temperature, pressure, altitude;
  Vector3 acceleration;

  parachute.getOrientation(yaw, pitch, roll);
  acceleration = parachute.getAcceleration();
  temperature = parachute.getTemperature();
  pressure = parachute.getPressure();
  altitude = parachute.getAltitude();

  // 5. GPS 데이터 수집
  double latitude = gps.getLatitude();
  double longitude = gps.getLongitude();
  double gpsAlt = gps.getAltitude();
  int sats = gps.getSatellites();

  // 6. SD 카드에 데이터 기록
  if (!logData(logFilename, yaw, pitch, roll,
               acceleration.x, acceleration.y, acceleration.z,
               temperature, pressure, altitude,
               latitude, longitude, gpsAlt, sats)) {
    Serial.println("Error: Failed to write data to SD card.");
  }

  // 7. 시리얼 출력 (CSV 형태)
  Serial.print(yaw, 2); Serial.print(",");
  Serial.print(pitch, 2); Serial.print(",");
  Serial.print(roll, 2); Serial.print(",");
  Serial.print(acceleration.x, 2); Serial.print(",");
  Serial.print(acceleration.y, 2); Serial.print(",");
  Serial.print(acceleration.z, 2); Serial.print(",");
  Serial.print(temperature, 2); Serial.print(",");
  Serial.print(pressure, 2); Serial.print(",");
  Serial.print(altitude, 2); Serial.print(",");
  Serial.print(latitude, 6); Serial.print(",");
  Serial.print(longitude, 6); Serial.print(",");
  Serial.print(gpsAlt, 2); Serial.print(",");
  Serial.println(sats);

  // 8. LoRa 무선 전송 (중요 정보만 간단히 전송)
  char message[100];
  snprintf(message, sizeof(message), "%.2f,%.2f,%.2f,%.6f,%.6f,%.2f",
           yaw, pitch, roll, latitude, longitude, gpsAlt);
  rf95.send((uint8_t *)message, strlen(message) + 1);
  rf95.waitPacketSent();

  // 9. 주기 조절
  delay(10);
}
