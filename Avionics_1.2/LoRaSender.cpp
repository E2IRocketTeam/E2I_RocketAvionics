#include "LoRaSender.h"

LoRaSender::LoRaSender(int csPin, int rstPin, int intPin, float frequency)
    : csPin(csPin), rstPin(rstPin), intPin(intPin), frequency(frequency), rf95(csPin, intPin) {}

bool LoRaSender::begin() {
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

    Serial.print("📡 주파수 설정: ");
    Serial.print(frequency);
    Serial.println(" MHz");
    
    rf95.setTxPower(23, false);
    return true;
}

void LoRaSender::sendData(float yaw, float pitch, float roll, float temperature, float pressure, float altitude) {
    char loRaMessage[80];
    snprintf(loRaMessage, sizeof(loRaMessage), "Logged Data: %.2f,%.2f,%.2f,%.2f,%.2f,%.2f", 
             yaw, pitch, roll, temperature, pressure, altitude);

    rf95.send((uint8_t *)loRaMessage, strlen(loRaMessage));
    rf95.waitPacketSent();

}
