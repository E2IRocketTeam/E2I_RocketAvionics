#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 915.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200); // RYLR896 모듈과 통신
    delay(100);
    
    Serial1.println("AT+BAND=868000000"); // RYLR896 주파수 설정
    delay(100);

    if (!rf95.init()) {
        Serial.println("LoRa receiver init failed");
        while (1);
    }
    Serial.println("LoRa receiver init OK!");

    if (!rf95.setFrequency(RF95_FREQ)) {
        Serial.println("setFrequency failed");
        while (1);
    }
    Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
    rf95.setTxPower(23, false);
}

void loop() {
    if (rf95.available()) {
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        if (rf95.recv(buf, &len)) {
            buf[len] = '\0';
            String receivedData = String((char*)buf);

            float yaw, pitch, roll, temperature, pressure, altitude;
            sscanf(receivedData.c_str(), "%f,%f,%f,%f,%f,%f", &yaw, &pitch, &roll, &temperature, &pressure, &altitude);

            Serial.print("Yaw: "); Serial.print(yaw);
            Serial.print(", Pitch: "); Serial.print(pitch);
            Serial.print(", Roll: "); Serial.println(roll);

            Serial.print("온도: "); Serial.print(temperature);
            Serial.print(", 기압: "); Serial.print(pressure);
            Serial.print(", 고도: "); Serial.println(altitude);
            Serial.println();
        }
    }

    // RESET 명령 전송 후에도 계속 수신
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        if (command == "RESET") {
            Serial.println("Sending RESET command to Transmitter...");
            rf95.send((uint8_t *)"RESET", 6);
            rf95.waitPacketSent();
        }
    }
}
