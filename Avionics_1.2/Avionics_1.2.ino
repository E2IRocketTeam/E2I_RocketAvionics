// E2I Rocket Avionics Main Control Code

#include "BNO085.h"
#include "BMP390.h"
#include "Sdcard.h"

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

}

void loop() {
  // 1. Update sensor values internally
  bno085.update();

  // Declare variables for sensor data
  float yaw, pitch, roll;
  float temperature, pressure, altitude;
  Vector3 acceleration;

  // 2. Read the updated values
  bno085.readData(yaw, pitch, roll);
  acceleration = bno085.getAccelerometer();
  bmp390.readData(temperature, pressure, altitude);

  // 3. Log data to SD card
  if (logData(logFilename, yaw, pitch, roll,
              acceleration.x, acceleration.y, acceleration.z,
              temperature, pressure, altitude)) {

    // Print current status to Serial Monitor for debugging
    Serial.print(yaw, 1); Serial.print(",");
    Serial.print(pitch, 1); Serial.print(",");
    Serial.print(roll, 1);Serial.print(",");
    Serial.print(acceleration.x, 2); Serial.print(",");
    Serial.print(acceleration.y, 2); Serial.print(",");
    Serial.print(acceleration.z, 2);Serial.print(",");
    Serial.print(temperature, 1); Serial.print("C,");
    Serial.print(pressure, 1); Serial.print("hPa,");
    Serial.print(altitude, 1); Serial.print("m");
    Serial.println();

  } else {
    Serial.println("Error: Failed to write data to SD card.");
  }

  // Set logging frequency
  delay(100);
}