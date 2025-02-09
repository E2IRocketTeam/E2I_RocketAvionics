#include "LoRaReceiver.h"

LoRaReceiver::LoRaReceiver(int csPin, int rstPin, int intPin, float frequency)
    : csPin(csPin), rstPin(rstPin), intPin(intPin), frequency(frequency), rf95(csPin, intPin) {}

bool LoRaReceiver::begin() {
    pinMode(rstPin, OUTPUT);
    digitalWrite(rstPin, HIGH);
    delay(10);

    // LoRa 모듈 리셋
    digitalWrite(rstPin, LOW);
    delay(10);
    digitalWrite(rstPin, HIGH);
    delay(10);

    if (!rf95.init()) {
        Serial.println("LoRa 모듈 초기화 실패!");
        return false;
    }

    if (!rf95.setFrequency(frequency)) {
        Serial.println("주파수 설정 실패!");
        return false;
    }

    Serial.print("주파수 설정: ");
    Serial.print(frequency);
    Serial.println(" MHz");

    rf95.setTxPower(23, false);
    return true;
}

bool LoRaReceiver::receiveData(char* receivedMessage, uint8_t maxLength, int &rssi) {
    if (rf95.available()) {
        uint8_t buf[maxLength];
        uint8_t len = maxLength;

        if (rf95.recv(buf, &len)) {
            strncpy(receivedMessage, (char*)buf, len);
            receivedMessage[len] = '\0';  // 문자열 종료

            rssi = rf95.lastRssi();
            return true;
        }
    }
    return false;
}
