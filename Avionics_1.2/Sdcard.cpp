#include "Sdcard.h"

bool initializeSD() {
  return SD.begin(BUILTIN_SDCARD);  // Teensy 기준
}

bool createLogFile(const char* filename) {
  File logFile = SD.open(filename, FILE_WRITE);
  if (!logFile) return false;

  logFile.println("Time,Yaw,Pitch,Roll,AccX,AccY,AccZ,Temp,Pressure,Alt,Lat,Lon,GPS_Alt,Sats");
  logFile.close();
  return true;
}

bool logData(const char* filename,
             float yaw, float pitch, float roll,
             float ax, float ay, float az,
             float temp, float pressure, float alt,
             double lat, double lon, double gpsAlt,
             int sats) {
  File logFile = SD.open(filename, FILE_WRITE);
  if (!logFile) return false;

  logFile.print(millis()); logFile.print(",");
  logFile.print(yaw); logFile.print(",");
  logFile.print(pitch); logFile.print(",");
  logFile.print(roll); logFile.print(",");
  logFile.print(ax); logFile.print(",");
  logFile.print(ay); logFile.print(",");
  logFile.print(az); logFile.print(",");
  logFile.print(temp); logFile.print(",");
  logFile.print(pressure); logFile.print(",");
  logFile.print(alt); logFile.print(",");
  logFile.print(lat, 6); logFile.print(",");
  logFile.print(lon, 6); logFile.print(",");
  logFile.print(gpsAlt); logFile.print(",");
  logFile.println(sats);

  logFile.close();
  return true;
}
