#define LORA Serial2  // Serial2 사용 (TX=8, RX=7)

void setup() {
    Serial.begin(9600);
    LORA.begin(9600);

    delay(500);
    Serial.println("LoRa 수신기 시작");

    // LoRa 모듈 주소 설정 (수신기: 주소 2)
    LORA.println("AT+ADDRESS=2");
    delay(500);
}

void loop() {
    if (LORA.available()) {
        String receivedData = LORA.readString();
        Serial.print("수신된 데이터: ");
        Serial.println(receivedData);
    }
}
