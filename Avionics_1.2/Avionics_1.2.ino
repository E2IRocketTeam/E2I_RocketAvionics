#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>
#include "Sdcard.h"
#include "Adafruit_BMP3XX.h"
#include "BNO055.h"
#include "Kalman.h"

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 915.0 // LoRa 주파수 설정

RH_RF95 rf95(RFM95_CS, RFM95_INT);

const char* filename = "sensor_data.csv";
Adafruit_BMP3XX bmpSensor;
BNO055 bnoSensor;
Kalman kalmanYaw, kalmanPitch, kalmanRoll;
unsigned long previousTime = 0;

void softwareReset() {
    Serial.println("Resetting Transmitter...");
    delay(1000);
    SCB_AIRCR = 0x05FA0004;  // Teensy에서 소프트웨어 리셋 실행
}

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200); // RYLR896 모듈과 통신
    delay(100);
    
    Serial1.println("AT+BAND=868000000"); // RYLR896 주파수 설정
    delay(100);

    if (!rf95.init()) {
        Serial.println("LoRa radio init failed");
        while (1);
    }
    Serial.println("LoRa radio init OK!");

    if (!rf95.setFrequency(RF95_FREQ)) {
        Serial.println("setFrequency failed");
        while (1);
    }
    Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
    rf95.setTxPower(23, false);

    // BMP390 센서 초기화
    if (!bmpSensor.begin_I2C()) { // I2C 모드로 초기화
        Serial.println("BMP390 센서 초기화 실패!");
        while (1);
    }

    if (!bnoSensor.begin()) {
        Serial.println("BNO055 센서 초기화 실패!");
        while (1);
    }

    Serial.println("센서 초기화 완료.");
}

void loop() {
    float filteredYaw, filteredPitch, filteredRoll;
    float temperature, pressure, altitude;

    // BNO055에서 Yaw, Pitch, Roll 데이터 읽기
    bnoSensor.readData(filteredYaw, filteredPitch, filteredRoll);

    // BMP390에서 온도, 기압, 고도 데이터 읽기
    bmpSensor.performReading();
    temperature = bmpSensor.temperature;
    pressure = bmpSensor.pressure / 100.0; // Pa -> hPa 변환
    altitude = bmpSensor.readAltitude(1013.25); // 기준 기압(1013.25 hPa) 적용

    Serial.print(filteredYaw); Serial.print(",");
    Serial.print(filteredPitch); Serial.print(",");
    Serial.println(filteredRoll); Serial.print(",");
    Serial.print(temperature); Serial.print(",");
    Serial.print(pressure); Serial.print(",");
    Serial.println(altitude);

    // 센서 데이터를 LoRa를 통해 송신
    char message[50];
    snprintf(message, sizeof(message), "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f", 
             filteredYaw, filteredPitch, filteredRoll, temperature, pressure, altitude);

    rf95.send((uint8_t *)message, strlen(message) + 1);
    rf95.waitPacketSent();

    // LoRa 메시지 수신 확인 (RESET 명령 수신 처리)
    if (rf95.available()) {
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        if (rf95.recv(buf, &len)) {
            buf[len] = '\0';
            String receivedData = String((char*)buf);
            Serial.print("Received: "); Serial.println(receivedData);

            if (receivedData == "RESET") {
                softwareReset();  // Teensy 소프트웨어 리셋 실행
            }
        }
    }

}
