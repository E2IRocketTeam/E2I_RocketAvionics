#ifndef SDCARD_H
#define SDCARD_H

#include <Arduino.h>
#include <SD.h>

bool initializeSD();
bool createLogFile(const char* filename);

bool logData(const char* filename,
             float yaw, float pitch, float roll,
             float accelX, float accelY, float accelZ,
             float temperature, float pressure, float altitude,
             double latitude, double longitude, double gpsAlt,
             int sats);

#endif
