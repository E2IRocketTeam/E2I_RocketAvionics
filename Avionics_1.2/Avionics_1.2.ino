#include <Wire.h>
#include "Sdcard.h"
#include "BMP390.h"
#include "BNO055.h"

const char* filename = "sensor_data.csv";
BMP390 bmpSensor;
BNO055 bnoSensor;

void setup() {
    Serial.begin(9600);
    while (!Serial);

    if (!initializeSD()) {
        Serial.println("SD card initialization failed!");
        while (1);
    }
    Serial.println("SD card initialized.");

    if (!bmpSensor.begin()) {
        Serial.println("BMP390 initialization failed!");
        while (1);
    }

    if (!bnoSensor.begin()) {
        Serial.println("BNO055 initialization failed!");
        while (1);
    }

    if (!createLogFile(filename)) {
        Serial.println("Failed to create log file!");
        while (1);
    }
}

void loop() {
    float temperature, pressure, altitude;
    float yaw, pitch, roll;

    bmpSensor.readData(temperature, pressure, altitude);
    bnoSensor.readData(yaw, pitch, roll);

    if (logData(filename, yaw, pitch, roll, temperature, pressure, altitude)) {
        Serial.print("Logged Data: ");
        Serial.print(yaw, 2); Serial.print(", ");
        Serial.print(pitch, 2); Serial.print(", ");
        Serial.print(roll, 2); Serial.print(", ");
        Serial.print(temperature, 2); Serial.print(", ");
        Serial.print(pressure, 2); Serial.print(", ");
        Serial.println(altitude, 2);
    } else {
        Serial.println("Failed to log data");
    }
}
