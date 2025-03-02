#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>
#include "Sdcard.h"
#include "BMP390.h"
#include "BNO055.h"
#include "Kalman.h"

// RFM95W 핀 정의
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 915.0 // 주파수 (모듈에 맞게 설정)


RH_RF95 rf95(RFM95_CS, RFM95_INT);

const char* filename = "sensor_data.csv";
BMP390 bmpSensor;
BNO055 bnoSensor;

Kalman kalmanYaw, kalmanPitch, kalmanRoll;
unsigned long previousTime = 0;

void setup() {
    Serial.begin(115200);
    delay(100);

    // LoRa 초기화 RFM
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);

    digitalWrite(RFM95_RST, LOW);
    delay(100);
    digitalWrite(RFM95_RST, HIGH);
    delay(100);

    // LoRa 초기화 RYLR896
    Serial1.begin(115200); 
    Serial1.println("AT");                // 모듈 응답 확인
    delay(50);
    Serial1.println("AT+ADDRESS=2");        // 수신기 주소 설정 (송신기와 맞춰야 함)
    delay(50);
    Serial1.println("AT+NETWORKID=5");      // 네트워크 ID 설정 (송신기와 동일)
    delay(50);
    Serial1.println("AT+BAND=868000000");   // 주파수 설정 (예: 868MHz)
    delay(50);


    if (!rf95.init()) {
        Serial.println("LoRa 모듈 초기화 실패!");
        while (1);
    }
    Serial.println("LoRa 모듈 초기화 성공!");

    if (!rf95.setFrequency(RF95_FREQ)) {
        Serial.println("LoRa 주파수 설정 실패!");
        while (1);
    }
    Serial.print("LoRa 주파수 설정 완료: "); Serial.println(RF95_FREQ);

    // LoRa 모듈 파라미터 최적화

    rf95.setTxPower(23, false);  // 송신 전력 최대 설정 (23dBm)

    // SD 카드 초기화
    if (!initializeSD()) {
        Serial.println("SD 카드 초기화 실패!");
        while (1);
    }

    // 센서 초기화
    if (!bmpSensor.begin()) {
        Serial.println("BMP390 센서 초기화 실패!");
        while (1);
    }

    if (!bnoSensor.begin()) {
        Serial.println("BNO055 센서 초기화 실패!");
        while (1);
    }

    if (!createLogFile(filename)) {
        Serial.println("로그 파일 생성 실패!");
        while (1);
    }

    float initYaw, initPitch, initRoll;
    bnoSensor.readData(initYaw, initPitch, initRoll);
    kalmanYaw.setAngle(initYaw);
    kalmanPitch.setAngle(initPitch);
    kalmanRoll.setAngle(initRoll);

    previousTime = millis(); // dt 계산을 위한 초기 시간 설정

    Serial.println("시스템 초기화 완료.");
}

void loop() {
    float temperature, pressure, altitude;
    float rawYaw, rawPitch, rawRoll;

    // BMP390 센서 데이터 읽기
    bmpSensor.readData(temperature, pressure, altitude);

    // BNO055 센서 데이터 읽기 (원본 각도값)
    bnoSensor.readData(rawYaw, rawPitch, rawRoll);

    unsigned long currentTime = millis();
    float dt = (currentTime - previousTime) / 1000.0f;
    previousTime = currentTime;

    // 칼만 필터 적용
    float filteredYaw = kalmanYaw.getAngle(rawYaw, 0, dt);
    float filteredPitch = kalmanPitch.getAngle(rawPitch, 0, dt);
    float filteredRoll = kalmanRoll.getAngle(rawRoll, 0, dt);

    // SD 카드에 로그 기록
    logData(filename, filteredYaw, filteredPitch, filteredRoll, temperature, pressure, altitude);

    // 시리얼 모니터에 데이터 출력
    Serial.print(filteredYaw); Serial.print(", ");
    Serial.print(filteredPitch); Serial.print(", ");
    Serial.print(filteredRoll); Serial.print(", ");
    Serial.print(temperature); Serial.print(", ");
    Serial.print(pressure); Serial.print(", ");
    Serial.println(altitude);

    if (Serial1.available()) {
        // 개행 문자('\n')까지 읽어서 문자열 생성
        String receivedMessage = Serial1.readStringUntil('\n');
        Serial.print("수신: ");
        Serial.println(receivedMessage);
      }
      
    // LoRa로 데이터 전송 (CSV 형식)
    char message[50]; // 작은 크기의 메시지를 사용
    snprintf(message, sizeof(message), "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
             filteredYaw, filteredPitch, filteredRoll, temperature, pressure, altitude);

    // 송신 간격을 최소화하기 위해 1ms마다 송신
    Serial.println(message);

    rf95.send((uint8_t *)message, strlen(message) + 1);

    // 송신 대기 시간 최소화
    unsigned long startTime = millis();
    while (!rf95.waitPacketSent()) {
        if (millis() - startTime > 50) { // 50ms 이상 대기하지 않도록
            Serial.println("송신 실패!");
            return;
        }
    }
}
