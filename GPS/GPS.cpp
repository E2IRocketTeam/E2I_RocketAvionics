#include "GPS.h"

GPS::GPS(HardwareSerial& serialPort) : gpsSerial(serialPort) {}

void GPS::begin(unsigned long baud) {
    gpsSerial.begin(baud);
}

void GPS::update() {
    while (gpsSerial.available()) {
        gps.encode(gpsSerial.read());
    }
}

bool GPS::isLocationUpdated() const {
    return gps.location.isUpdated();
}

double GPS::getLatitude() {
    return gps.location.lat();  // const 제거
}

double GPS::getLongitude() {
    return gps.location.lng();  // const 제거
}

double GPS::getAltitude() {
    return gps.altitude.meters();  // const 제거
}

int GPS::getSatellites() {
    return gps.satellites.value();  // const 제거
}

String GPS::getDate() {
    if (!gps.date.isValid()) return "INVALID";

    char buffer[11];
    sprintf(buffer, "%04d/%02d/%02d",
            gps.date.year(),
            gps.date.month(),
            gps.date.day());
    return String(buffer);
}

String GPS::getTime() {
    if (!gps.time.isValid()) return "INVALID";  // 세미콜론 추가됨

    char buffer[12];
    sprintf(buffer, "%02d:%02d:%02d",
            gps.time.hour(),
            gps.time.minute(),
            gps.time.second());
    return String(buffer);
}
