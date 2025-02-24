#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

#define RF95_FREQ 915.0  // 주파수 (수신기와 동일해야 함)

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);

    Serial.begin(9600);
    while (!Serial);
    Serial.println("LoRa Sender Initializing...");

    // LoRa 모듈 리셋
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);

    // LoRa 모듈 초기화
    if (!rf95.init()) {
        Serial.println("LoRa radio init failed ❌");
        while (1);
    }
    Serial.println("LoRa radio init OK ✅");

    // 주파수 설정
    if (!rf95.setFrequency(RF95_FREQ)) {
        Serial.println("Set frequency failed ❌");
        while (1);
    }
    Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

    // 송신 출력 파워 설정
    rf95.setTxPower(23, false);
}

void loop() {
    Serial.println("Sending: Hello LoRa");

    // 송신할 메시지 설정
    const char message[] = "아주좋아요";

    Serial.println("Calling rf95.send()...");  // 디버깅용 출력
    rf95.send((uint8_t *)message, sizeof(message));
    
    Serial.println("Waiting for packet to be sent...");  // 디버깅용 출력
    rf95.waitPacketSent();
    
    Serial.println("Message Sent! ✅");  // 메시지 전송 완료 확인

    // 2초마다 메시지 전송
    delay(2000);
}
