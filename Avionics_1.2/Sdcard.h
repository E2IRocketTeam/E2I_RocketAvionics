#ifndef SDCARD_H
#define SDCARD_H

#include <SD.h>

const int chipSelect = BUILTIN_SDCARD;

bool initializeSD();
bool createLogFile(const char* filename);
bool logData(const char* filename, float yaw, float pitch, float roll, float temperature, float pressure, float altitude);

#endif
