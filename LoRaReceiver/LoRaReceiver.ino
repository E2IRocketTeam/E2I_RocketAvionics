#include "LoRaReceiver.h"

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 920.9  // 주파수 920.9 MHz

LoRaReceiver loRa(RFM95_CS, RFM95_RST, RFM95_INT, RF95_FREQ);

void setup() {
    Serial.begin(115200);
    while (!Serial);

    if (!loRa.begin()) {
        Serial.println("LoRa 모듈 초기화 실패!");
        while (1);
    }
}

void loop() {
    char receivedMessage[80];
    int rssi;

    if (loRa.receiveData(receivedMessage, sizeof(receivedMessage), rssi)) {
        Serial.print("받은 데이터: ");
        Serial.println(receivedMessage);
        Serial.print("RSSI: ");
        Serial.println(rssi);
    }
}
