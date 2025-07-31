#include "Sdcard.h"

bool initializeSD() {
    if (!SD.begin(chipSelect)) {
        return false;
    }
    return true;
}

bool createLogFile(const char* filename) {
    
     if (SD.exists(filename)) {
        SD.remove(filename);  // 이전 내용 삭제 추가 !
    }
    File dataFile = SD.open(filename, FILE_WRITE);
    if (dataFile) {
        // CSV 파일 헤더를 요청하신 순서대로 변경합니다.
        dataFile.println("Yaw,Pitch,Roll,AccelX,AccelY,AccelZ,Temperature,Pressure,Altitude");
        dataFile.close();
        return true;
    }
    return false;
}

// 함수의 정의와 데이터 기록 순서를 요청하신 대로 변경합니다.
bool logData(const char* filename, float yaw, float pitch, float roll, float accelX, float accelY, float accelZ, float temperature, float pressure, float altitude) {
    File dataFile = SD.open(filename, FILE_WRITE);
    if (dataFile) {
        // 1. 자세 데이터
        dataFile.print(yaw, 2); dataFile.print(",");
        dataFile.print(pitch, 2); dataFile.print(",");
        dataFile.print(roll, 2); dataFile.print(",");

        // 2. 가속도 데이터
        dataFile.print(accelX, 4); dataFile.print(",");
        dataFile.print(accelY, 4); dataFile.print(",");
        dataFile.print(accelZ, 4); dataFile.print(",");

        // 3. 환경 데이터
        dataFile.print(temperature, 2); dataFile.print(",");
        dataFile.print(pressure, 2); dataFile.print(",");
        dataFile.println(altitude, 2); // 마지막 값

        dataFile.close();
        return true;
    }
    return false;
}
