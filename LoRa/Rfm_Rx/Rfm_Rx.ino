#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

// 주파수 설정 (수신자와 동일해야 함)
#define RF95_FREQ 915.0

// RF95 객체 생성
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// LED 핀 정의
#define LED 13

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  Serial.begin(9600);
  delay(100);

  Serial.println("LoRa Receiver Starting...");

  // RFM95 초기화
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // 주파수 설정
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); 
  Serial.println(RF95_FREQ);

  // 송신 전력 설정 (기본값은 13dBm)
  rf95.setTxPower(13, false);
}

void loop()
{
  // 메시지가 수신되었는지 확인
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    // 메시지 수신
    if (rf95.recv(buf, &len)) {
      digitalWrite(LED, HIGH);
      Serial.print("Received: ");
      Serial.write(buf, len);
      Serial.println();
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
      digitalWrite(LED, LOW);
    } else {
      Serial.println("Receive failed");
    }
  }
}
