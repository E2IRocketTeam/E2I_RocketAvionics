// E2I Rocket Avionics Main Control Code (v1.4 - Stabilization Delay)
#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>

#include "Parachute.h"
#include "Sdcard.h"

// --- Pin & Frequency Definitions ---
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 915.0
const int SERVO_PIN = 22;

// --- Object Creation ---
RH_RF95 rf95(RFM95_CS, RFM95_INT);
Parachute parachute(SERVO_PIN);

// --- SD Card & Timing Settings ---
const char* logFilename = "flight_log.csv";
unsigned long programStartTime = 0; // 프로그램 시작 시간 저장을 위한 변수

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("===== Avionics System Start (v1.4) =====");

  // Parachute 시스템 초기화
  parachute.begin();

  // SD Card 초기화
  if (!initializeSD()) {
    Serial.println("Error: SD card initialization failed!");
    while (1) delay(10);
  }
  if (!createLogFile(logFilename)) {
    Serial.println("Error: Failed to create log file!");
    while (1) delay(10);
  }

  // LoRa 통신 초기화
  if (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  rf95.setFrequency(RF95_FREQ);
  rf95.setTxPower(23, false);

  // --- 프로그램 시작 시간 기록 ---
  programStartTime = millis();
  Serial.println("Sensor stabilization period started (15 seconds)...");
}

void loop() {
  // 1. 낙하산 시스템은 항상 업데이트 (내부 상태 계산을 위해)
  parachute.update();

  // 2. 15초 안정화 시간 확인
  if (millis() - programStartTime < 5000) {
    Serial.print("."); // 대기 중임을 시리얼 모니터에 표시
    delay(500);         // 너무 많은 메시지 출력을 방지
    return; // 15초가 지나지 않았으면 루프의 나머지 부분을 실행하지 않음
  }
  
  // --- 15초 경과 후 아래 코드 실행 ---

  // 3. Parachute 객체로부터 데이터 가져오기
  float yaw, pitch, roll, temperature, pressure, altitude;
  Vector3 acceleration;

  parachute.getOrientation(yaw, pitch, roll);
  acceleration = parachute.getAcceleration();
  temperature = parachute.getTemperature();
  pressure = parachute.getPressure();
  altitude = parachute.getAltitude();
  
  // 4. SD 카드에 데이터 기록
  if (!logData(logFilename, yaw, pitch, roll,
              acceleration.x, acceleration.y, acceleration.z,
              temperature, pressure, altitude)) {
    Serial.println("Error: Failed to write data to SD card.");
  }

  // 5. 시리얼 모니터에 데이터 출력
    Serial.print(yaw, 2); Serial.print(",");
    Serial.print(pitch, 2); Serial.print(",");
    Serial.print(roll, 2);Serial.print(",");
    Serial.print(acceleration.x, 2); Serial.print(",");
    Serial.print(acceleration.y, 2); Serial.print(",");
    Serial.print(acceleration.z, 2);Serial.print(",");
    Serial.print(temperature, 2); Serial.print(",");
    Serial.print(pressure, 2); Serial.print(",");
    Serial.print(altitude, 2);
    Serial.println();

  // 6. LoRa로 데이터 전송
  char message[50];
  snprintf(message, sizeof(message), "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f", 
           yaw, pitch, roll, temperature, pressure, altitude);
  rf95.send((uint8_t *)message, strlen(message) + 1);
  rf95.waitPacketSent();
    
  // 루프 주기 설정
  delay(10);
}