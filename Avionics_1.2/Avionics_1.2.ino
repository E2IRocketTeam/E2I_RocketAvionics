#include <SPI.h>
#include <RH_RF95.h> 
#include <Wire.h> 
#include "Sdcard.h" 
#include "BMP390.h"
#include "BNO085.h"
#include "Kalman.h"

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 915.0 // RF95W Frequency setting (915MHz)

RH_RF95 rf95(RFM95_CS, RFM95_INT);

const char* filename = "sensor_data.csv";
BNO085 bnoSensor;
BMP390 bmpSensor;
Kalman kalmanYaw, kalmanPitch, kalmanRoll;
unsigned long previousTime = 0;

void softwareReset() {
    Serial.println("Resetting Transmitter...");
    delay(1000);
    SCB_AIRCR = 0x05FA0004;  // Teensy software reset 
}

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200); // RYLR896 setting
    delay(100);
    
    Serial1.println("AT+BAND=868000000"); // RYLR896 Frequency setting (868MHz)
    delay(100);

    if (!rf95.init()) {
        Serial.println("LoRa radio init failed");
        while (1);
    }
    Serial.println("LoRa radio init OK!");

    if (!rf95.setFrequency(RF95_FREQ)) {
        Serial.println("setFrequency failed");
        while (1);
    }
    Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
    rf95.setTxPower(23, false);

    // BMP390 setting reset
    if (!bmpSensor.begin()) { // I2C mode reset
        Serial.println("BMP390 센서 초기화 실패!");
        while (1);
    }

    if (!bnoSensor.begin()) {
        Serial.println("BNO085 센서 초기화 실패!");
        while (1);
    }

    Serial.println("센서 초기화 완료.");
}

void loop() {
    float filteredYaw, filteredPitch, filteredRoll;
    float temperature, pressure, altitude;

    // Yaw, Pitch, Roll data reading From BNO085
    bnoSensor.readData(filteredYaw, filteredPitch, filteredRoll);

    // Temperature, Pressure, altitude From BMP390
    bmpSensor.readData(temperature, pressure, altitude);

    Serial.print(filteredYaw); Serial.print(",");
    Serial.print(filteredPitch); Serial.print(",");
    Serial.print(filteredRoll); Serial.print(",");
    Serial.print(temperature); Serial.print(",");
    Serial.print(pressure); Serial.print(",");
    Serial.println(altitude);

    // Transmit sensor data through LoRa
    char message[50];
    snprintf(message, sizeof(message), "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f", 
             filteredYaw, filteredPitch, filteredRoll, temperature, pressure, altitude);

    rf95.send((uint8_t *)message, strlen(message) + 1);
    rf95.waitPacketSent();

    // LoRa message check (RESET Receiving)
    if (rf95.available()) {
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        if (rf95.recv(buf, &len)) {
            buf[len] = '\0';
            String receivedData = String((char*)buf);
            Serial.print("Received: "); Serial.println(receivedData);

            if (receivedData == "RESET") {
                softwareReset();  // Teensy SOftware Reset Operation
            }
        }
    }


}
