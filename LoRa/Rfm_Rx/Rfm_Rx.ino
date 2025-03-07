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

  Serial.begin(115200);
  delay(100);

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
  rf95.setTxPower(23, false);
}

void loop()
{
  // 메시지가 수신되었는지 확인
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    // 메시지 수신
    if (rf95.recv(buf, &len)) {
      buf[len] = '\0';  // 문자열 끝을 명확히 지정
      String receivedData = String((char*)buf);

      // 쉼표(,)를 기준으로 데이터를 분리
      float yaw, pitch, roll, temperature, pressure, altitude;
      sscanf(receivedData.c_str(), "%f,%f,%f,%f,%f,%f", &yaw, &pitch, &roll, &temperature, &pressure, &altitude);

      // 원하는 형식으로 시리얼 모니터 출력
      Serial.print("Yaw: "); Serial.print(yaw);
      Serial.print(", Pitch: "); Serial.print(pitch);
      Serial.print(", Roll: "); Serial.println(roll);
      
      Serial.print("온도: "); Serial.print(temperature);
      Serial.print(", 기압: "); Serial.print(pressure);
      Serial.print(", 고도: "); Serial.println(altitude);
      Serial.println(); // 줄 바꿈

    } else {
      Serial.println("Receive failed");
    }
  }
}
