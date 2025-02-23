#include <SPI.h>
#include <RH_RF95.h>

// RFM95W 핀 정의
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 915.0 // 주파수 (송신기와 동일하게 설정)

RH_RF95 rf95(RFM95_CS, RFM95_INT); // RFM95 객체 생성

void setup() {
    Serial.begin(115200); // 시리얼 통신 시작
    delay(100);

    // LoRa 초기화
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);
    digitalWrite(RFM95_RST, LOW); // RFM95W 리셋
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);

    if (!rf95.init()) {
        Serial.println("RFM95W 초기화 실패!");
        while (1);
    }

    if (!rf95.setFrequency(RF95_FREQ)) {
        Serial.println("LoRa 주파수 설정 실패!");
        while (1);
    }
    rf95.setTxPower(23, false); // 출력 전력 설정
    Serial.println("LoRa 수신기 초기화 완료.");
}

void loop() {
    if (rf95.available()) {
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);

        // 메시지 수신
        if (rf95.recv(buf, &len)) {
            Serial.print("수신된 메시지: ");
            buf[len] = '\0'; // Null 문자 추가하여 문자열로 변환
            Serial.println((char*)buf);

            // 메시지를 분석하여 데이터 추출
            parseLoRaData((char*)buf);
        } else {
            Serial.println("메시지 수신 오류!");
        }
    }
}

// 수신된 데이터를 파싱하는 함수
void parseLoRaData(char* message) {
    // CSV 형식: yaw,pitch,roll,temperature,pressure,altitude
    float yaw, pitch, roll, temperature, pressure, altitude;
    sscanf(message, "%f,%f,%f,%f,%f,%f", &yaw, &pitch, &roll, &temperature, &pressure, &altitude);

    // 파싱된 데이터 출력
    Serial.println("파싱된 데이터:");
    Serial.print("Yaw: "); Serial.println(yaw);
    Serial.print("Pitch: "); Serial.println(pitch);
    Serial.print("Roll: "); Serial.println(roll);
    Serial.print("Temperature: "); Serial.println(temperature);
    Serial.print("Pressure: "); Serial.println(pressure);
    Serial.print("Altitude: "); Serial.println(altitude);
}
