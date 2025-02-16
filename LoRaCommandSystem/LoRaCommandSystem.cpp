#include "LoRaCommandSystem.h"

LoRaCommandSystem::LoRaCommandSystem(Stream &loraStream, ForcedParachute &parachute)
    : loraStream(loraStream), parachute(parachute) {}

void LoRaCommandSystem::begin() {
    loraStream.begin(9600);      // LoRa 통신 초기화
    loraStream.println("AT+ADDRESS=2"); // 수신기 주소 설정
    parachute.begin();          // 서보모터 초기화
}

void LoRaCommandSystem::processCommands() {
    if (loraStream.available()) {
        String receivedData = loraStream.readString();
        receivedData.trim(); // 개행 문자 제거

        if (receivedData == PARACHUTE_OPEN_KEY) {
            parachute.openParachute();
        } else if (receivedData == PARACHUTE_CLOSE_KEY) {
            parachute.closeParachute();
        } else if (receivedData == RESET_KEY) {
            delay(500);
            softwareReset();
        }
    }
}

void LoRaCommandSystem::softwareReset() {
    SCB_AIRCR = 0x05FA0004 | (1 << 2); // System Reset Request (SYSRESETREQ)
}
