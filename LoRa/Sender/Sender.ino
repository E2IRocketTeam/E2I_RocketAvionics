void setup() {
    Serial.begin(9600);   // 디버깅용 USB Serial
    Serial1.begin(9600);  // RA-07 기본 통신 속도
  
    Serial.println("Teensy 4.1 LoRa 수신 초기화 중...");
  
    // 주파수 설정 (920 MHz)
    Serial1.println("AT+FREQ=920000");
    delay(100);
  
    // 송신 전력 설정 (10 dBm)
    Serial1.println("AT+POWER=10");
    delay(100);
  
    // 설정 확인
    Serial1.println("AT+FREQ?");
    delay(100);
    Serial1.println("AT+POWER?");
    delay(100);
  
    Serial.println("RA-07 설정 완료. 수신 대기 중...");
  }
  
  void loop() {
    // RA-07에서 데이터 읽기
    if (Serial1.available()) {
      String data = Serial1.readStringUntil('\n'); // 수신 데이터 읽기
      Serial.print("수신한 데이터: ");
      Serial.println(data);
    }
  }
  