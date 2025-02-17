#include <Wire.h>
#include "Sdcard.h"
#include "BMP390.h"
#include "BNO055.h"
#include "Parachute.h"
#include "Kalman.h"  


// #define RFM95_CS 10
// #define RFM95_RST 9
// #define RFM95_INT 2
// #define RF95_FREQ 920.9  // 주파수 920.9 MHz
 

#define LORA Serial2

Parachute parachute1(9);

const char* filename = "sensor_data.csv";
//LoRaSender LoRa(RFM95_CS, RFM95_RST, RFM95_INT, RF95_FREQ);
BMP390 bmpSensor;
BNO055 bnoSensor;

// yaw, pitch, roll 각각에 대해 칼만 필터 객체 생성
Kalman kalmanYaw, kalmanPitch, kalmanRoll;

// dt 계산을 위한 시간 변수
unsigned long previousTime = 0;

void setup() {
    Serial.begin(115200);  // 시리얼 통신 시작 (보드레이트: 115200)
    delay(100);  // SD 카드 안정성을 위한 초기 지연

    if (!initializeSD()) {
        Serial.println("SD 카드 초기화 실패!");
        while (1);
    }

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

    parachute1.begin();

    // 센서에서 초기 각도값 읽어 칼만 필터 초기값 설정
    float initYaw, initPitch, initRoll;
    bnoSensor.readData(initYaw, initPitch, initRoll);
    kalmanYaw.setAngle(initYaw);
    kalmanPitch.setAngle(initPitch);
    kalmanRoll.setAngle(initRoll);

    previousTime = millis();  // dt 계산을 위한 초기 시간 설정

    Serial.println("시스템 초기화 완료.");
}

void loop() {
    float temperature, pressure, altitude;
    float rawYaw, rawPitch, rawRoll;
    // BMP390 센서 데이터 읽기
    bmpSensor.readData(temperature, pressure, altitude);
    // BNO055 센서 데이터 읽기 (원본 각도값)
    bnoSensor.readData(rawYaw, rawPitch, rawRoll);
    parachute1.update();

    // 시간 차이 계산 (초 단위)
    unsigned long currentTime = millis();
    float dt = (currentTime - previousTime) / 1000.0f;
    previousTime = currentTime;

    // 칼만 필터 적용 (gyro 데이터가 없는 경우 0 사용)
    float filteredYaw   = kalmanYaw.getAngle(rawYaw,   0, dt);
    float filteredPitch = kalmanPitch.getAngle(rawPitch, 0, dt);
    float filteredRoll  = kalmanRoll.getAngle(rawRoll,  0, dt);

    // SD 카드에 로그 기록
    logData(filename, filteredYaw, filteredPitch, filteredRoll, temperature, pressure, altitude);

    // 시리얼 모니터에 원본 및 필터 적용 값 출력 (Python에서 데이터 읽을 때 사용)
    Serial.print(rawYaw); Serial.print(", ");   // 원본 Yaw
    Serial.print(rawPitch); Serial.print(", "); // 원본 Pitch
    Serial.print(rawRoll); Serial.print(", ");  // 원본 Roll
    Serial.print(filteredYaw); Serial.print(", ");   // 필터 적용 Yaw
    Serial.print(filteredPitch); Serial.print(", "); // 필터 적용 Pitch
    Serial.print(filteredRoll); Serial.print(", ");  // 필터 적용 Roll
    Serial.print(temperature); Serial.print(", ");
    Serial.print(pressure); Serial.print(", ");
    Serial.println(altitude);
}
