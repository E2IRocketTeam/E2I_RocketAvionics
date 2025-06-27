#ifndef SDCARD_H
#define SDCARD_H

#include <SD.h>

const int chipSelect = BUILTIN_SDCARD;

bool initializeSD();
bool createLogFile(const char* filename);
// 가속도 데이터를 받을 수 있도록 인자 추가
bool logData(const char* filename, float yaw, float pitch, float roll, float XAcc, float YAcc, float ZAcc, float temperature, float pressure, float altitude);

#endif 