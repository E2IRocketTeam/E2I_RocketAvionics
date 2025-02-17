#include <SoftwareSerial.h>

// LoRa와 통신을 위한 SoftwareSerial 설정
SoftwareSerial loraSerial(3, 1); // RX: GPIO3, TX: GPIO1

void setup() {
  Serial.begin(9600);   // 디버깅용 Serial Monitor
  loraSerial.begin(9600); // RA-07 기본 통신 속도

  Serial.println("ESP8266 LoRa 송신 초기화 중...");

  // 주파수 설정 (920 MHz)
  loraSerial.println("AT+FREQ=920000");
  delay(100);
  
  // 송신 전력 설정 (10 dBm)
  loraSerial.println("AT+POWER=10");
  delay(100);

  // 설정 확인
  loraSerial.println("AT+FREQ?");
  delay(100);
  loraSerial.println("AT+POWER?");
  delay(100);

  Serial.println("RA-07 설정 완료. 송신 준비 중...");
}

void loop() {
  // LoRa로 메시지 송신
  loraSerial.println("Hello from ESP8266"); // 메시지 송신
  Serial.println("메시지 전송: Hello from ESP8266");
  delay(2000); // 2초 간격
}
