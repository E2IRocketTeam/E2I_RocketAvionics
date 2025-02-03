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
        dataFile.println("Yaw,Pitch,Roll,Temperature,Pressure,Altitude");
        dataFile.close();
        return true;
    }
    return false;
}

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
    }
    return false;
}
