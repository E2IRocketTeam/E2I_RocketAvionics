#ifndef GPS_H
#define GPS_H

#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

class GPS {
public:
    GPS(HardwareSerial& serialPort);

    void begin(unsigned long baud = 9600);
    void update();
    bool isLocationUpdated() const;

    double getLatitude();    // const 제거
    double getLongitude();   // const 제거
    double getAltitude();    // const 제거
    int getSatellites();     // const 제거

    String getDate();        // const 제거
    String getTime();        // const 제거

private:
    TinyGPSPlus gps;
    HardwareSerial& gpsSerial;
};

#endif
