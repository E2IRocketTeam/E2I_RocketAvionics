#define LORA Serial2  // Serial2 사용 (TX=8, RX=7)

void setup() {
    Serial.begin(9600);  // PC와 연결된 USB 시리얼 모니터
    LORA.begin(9600);    // LoRa 모듈과 통신할 Serial2 시작

    delay(500);
    Serial.println("LoRa 송신기 시작");

    // LoRa 모듈 주소 설정 (송신기: 주소 1)
    LORA.println("AT+ADDRESS=1");
    delay(500);
}

void loop() {
    Serial.println("메시지 전송: Hello LoRa");
    LORA.println("AT+SEND=2,10,Hello LoRa");  // 주소 2로 메시지 전송
    delay(2000);  // 2초마다 전송
}
