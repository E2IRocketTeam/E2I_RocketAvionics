// E2I Rocket Avionics Main Control Code
#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>

#include "BNO085.h"
#include "BMP390.h"
#include "Sdcard.h"

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 915.0 // RF95W frequency setting (915MHz)

RH_RF95 rf95(RFM95_CS, RFM95_INT);
// Create sensor objects
BNO085 bno085;
BMP390 bmp390;

const char* logFilename = "flight_log.csv";

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("===== Avionics System Start =====");

  // Initialize BNO085
  Serial.println("Initializing BNO085 IMU...");
  if (!bno085.begin()) {
    Serial.println("Error: BNO085 sensor not found!");
    while (1) delay(10);
  }
  Serial.println("BNO085 initialized.");
  // Initialize BMP390
  Serial.println("Initializing BMP390 pressure sensor...");
  if (!bmp390.begin()) {
    Serial.println("Error: BMP390 sensor not found!");
    while (1) delay(10);
  }
  Serial.println("BMP390 initialized.");

  // Initialize SD Card and create log file
  Serial.println("Initializing SD card...");
  if (!initializeSD()) {
    Serial.println("Error: SD card initialization failed!");
    while (1) delay(10);
  }
  Serial.println("SD card initialized.");
  Serial.print("Creating log file: ");
  Serial.println(logFilename);
  if (!createLogFile(logFilename)) {
    Serial.println("Error: Failed to create log file!");
    while (1) delay(10);
  }

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
}

void loop() {
  // 1. Update sensor values internally
  bno085.update();
  
  // Declare variables for sensor data
  float yaw, pitch, roll;
  float temperature, pressure, altitude;
  Vector3 acceleration;
  
  // 2. Read the updated values using getter functions
  yaw = bno085.getYaw();
  pitch = bno085.getPitch();
  roll = bno085.getRoll();
  acceleration = bno085.getAccelerometer();
  bmp390.readData(temperature, pressure, altitude);
  
  // 3. Log data to SD card
  if (logData(logFilename, yaw, pitch, roll,
              acceleration.x, acceleration.y, acceleration.z,
              temperature, pressure, altitude)) {

    // Print current status to Serial Monitor for debugging
    Serial.print(yaw, 2); Serial.print(",");
    Serial.print(pitch, 2); Serial.print(",");
    Serial.print(roll, 2);Serial.print(",,,,,");
    Serial.print(acceleration.x, 2); Serial.print(",,,,,");
    Serial.print(acceleration.y, 2); Serial.print(",,,,,");
    Serial.print(acceleration.z, 2);Serial.print(",,,,,");
    Serial.print(temperature, 2); Serial.print(",");
    Serial.print(pressure, 2); Serial.print(",");
    Serial.print(altitude, 2);
    Serial.println();

  } else {
    Serial.println("Error: Failed to write data to SD card.");
  }

    char message[50];
    snprintf(message, sizeof(message), "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f", 
             yaw, pitch, roll
        , temperature, pressure, altitude);
    rf95.send((uint8_t *)message, strlen(message) + 1);
    rf95.waitPacketSent();
    
  // Set logging frequency
  delay(100);
}