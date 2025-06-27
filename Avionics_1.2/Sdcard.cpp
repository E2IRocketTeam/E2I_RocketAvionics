#include "Sdcard.h"

bool initializeSD() {
    if (!SD.begin(chipSelect)) {
        return false;
    }
    return true;
}

bool createLogFile(const char* filename) {
    File dataFile = SD.open(filename, FILE_WRITE);
    if (dataFile) {
        // CSV 헤더에 가속도 항목(AccelX, AccelY, AccelZ) 추가
        dataFile.println("Yaw,Pitch,Roll,Temperature,Pressure,Altitude,AccelX,AccelY,AccelZ");
        dataFile.close();
        return true;
    }
    return false;
}

// 가속도 데이터를 파일에 기록하는 로직 추가
bool logData(const char* filename, float yaw, float pitch, float roll, float accelX, float accelY, float accelZ, float temperature, float pressure, float altitude) {
    File dataFile = SD.open(filename, FILE_WRITE);
    if (dataFile) {
        dataFile.print(yaw, 3); dataFile.print(",");
        dataFile.print(pitch, 3); dataFile.print(",");
        dataFile.print(roll, 3); dataFile.print(",");
        dataFile.print(accelX, 4); dataFile.print(",");
        dataFile.print(accelY, 4); dataFile.print(",");
        dataFile.print(accelZ, 4);dataFile.print(",");
        dataFile.print(temperature, 3); dataFile.print(",");
        dataFile.print(pressure, 3); dataFile.print(",");
        dataFile.println(altitude, 3); 

        dataFile.close();
        return true;
    }
    return false;
}