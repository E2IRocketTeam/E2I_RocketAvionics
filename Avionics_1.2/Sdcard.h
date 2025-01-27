#ifndef SDCARD_H
#define SDCARD_H

#include <SD.h>

// SD 카드 칩 선택 핀 설정 (Teensy 4.1 내장 SD는 BUILTIN_SDCARD 사용)
const int chipSelect = BUILTIN_SDCARD;

// SD 카드 초기화 함수
bool initializeSD() {
  if (!SD.begin(chipSelect)) {
    return false;
  }
  return true;
}

// 데이터 파일 열기 및 헤더 작성
bool createLogFile(const char* filename) {
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    dataFile.println("Yaw,Pitch,Roll,Temperature,Pressure,Altitude"); // CSV 헤더 작성
    dataFile.close();
    return true;
  } else {
    return false;
  }
}

// 데이터 저장 함수
bool logData(const char* filename, float yaw, float pitch, float roll, float temperature, float pressure, float altitude) {
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    dataFile.print(yaw, 2);
    dataFile.print(",");
    dataFile.print(pitch, 2);
    dataFile.print(",");
    dataFile.print(roll, 2);
    dataFile.print(",");
    dataFile.print(temperature, 2);
    dataFile.print(",");
    dataFile.print(pressure, 2);
    dataFile.print(",");
    dataFile.println(altitude, 2);
    dataFile.close();
    return true;
  } else {
    return false;
  }
}

#endif