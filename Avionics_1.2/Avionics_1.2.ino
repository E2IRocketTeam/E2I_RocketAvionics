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

//RH_RF95 rf95(RFM95_CS, RFM95_INT); // RFM95 객체 생성


const char* filename = "sensor_data.csv";
BMP390 bmpSensor;
BNO055 bnoSensor;

// yaw, pitch, roll 각각에 대해 칼만 필터 객체 생성
Kalman kalmanYaw, kalmanPitch, kalmanRoll;

// dt 계산을 위한 시간 변수
unsigned long previousTime = 0;

void setup() {
    Serial.begin(115200); // 시리얼 통신 시작 (보드레이트: 115200)
    delay(100);           // 초기 안정성을 위한 지연

    // LoRa 초기화
    // pinMode(RFM95_RST, OUTPUT);
    // digitalWrite(RFM95_RST, HIGH);
    // delay(10);
    // digitalWrite(RFM95_RST, LOW); // RFM95W 리셋
    // delay(10);
    // digitalWrite(RFM95_RST, HIGH);
    // delay(10);

    // if (!rf95.init()) {
    //     Serial.println("RFM95W 초기화 실패!");
    //     while (1);
    // }

    // if (!rf95.setFrequency(RF95_FREQ)) {
    //     Serial.println("LoRa 주파수 설정 실패!");
    //     while (1);
    // }
    // rf95.setTxPower(23, false); // 출력 전력 설정

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



    // 센서에서 초기 각도값 읽어 칼만 필터 초기값 설정
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



    // 시간 차이 계산 (초 단위)
    unsigned long currentTime = millis();
    float dt = (currentTime - previousTime) / 1000.0f;
    previousTime = currentTime;

    // 칼만 필터 적용 (gyro 데이터가 없는 경우 0 사용)
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

    // LoRa로 데이터 전송
    //sendLoRaData(filteredYaw, filteredPitch, filteredRoll, temperature, pressure, altitude);
}

// LoRa 데이터를 송신하는 함수
// void sendLoRaData(float yaw, float pitch, float roll, float temperature, float pressure, float altitude) {
//     char message[100];
//     snprintf(message, sizeof(message), "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
//              yaw, pitch, roll, temperature, pressure, altitude);

//     rf95.send((uint8_t*)message, strlen(message)); // LoRa 데이터 송신
//     rf95.waitPacketSent();                         // 패킷 송신 완료 대기
//     Serial.println("LoRa 데이터 전송 완료: " + String(message));
// }
