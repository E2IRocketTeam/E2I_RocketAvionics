#include <Arduino.h>
#include "ForcedParachute.h"

#define PARACHUTE_OPEN_KEY "OP";
#define RESET_KEY "R";

#define LORA Serial2  // LoRa 통신을 위한 Serial2 (TX=8, RX=7)

// 소프트웨어 리셋 함수
void softwareReset() {
    SCB_AIRCR = 0x05FA0004 | (1 << 2);  // System Reset Request (SYSRESETREQ)
}

void setup() {
    Serial.begin(115200);
    LORA.begin(9600);
    // LoRa 모듈 주소 설정 (수신기: 주소 2)
    LORA.println("AT+ADDRESS=2");
    ForcedParachute parachute(9);
}

void loop() {
    if (LORA.available()) {
        String receivedData = LORA.readString();
        receivedData.trim();  // 개행 문자 제거

        if(receivedData == PARACHUTE_OPEN_KEY) {

        }

        if (receivedData == RESET_KEY) {
            softwareReset();
        }
    }
}
