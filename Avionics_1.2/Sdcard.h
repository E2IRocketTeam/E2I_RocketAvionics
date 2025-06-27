#ifndef SDCARD_H
#define SDCARD_H

#include <SD.h>

const int chipSelect = BUILTIN_SDCARD;

bool initializeSD();
bool createLogFile(const char* filename);

// 요청하신 순서대로 함수의 인자 순서를 변경합니다.
bool logData(const char* filename, float yaw, float pitch, float roll, float accelX, float accelY, float accelZ, float temperature, float pressure, float altitude);

#endif