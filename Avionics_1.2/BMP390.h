#ifndef BMP390_H
#define BMP390_H

#include <Arduino.h>
#include <Adafruit_BMP3XX.h>
#include "Sdcard.h"

class BMP390 {
public:
    BMP390(uint8_t address = 0x77);
    bool begin();
    void readSensorData();
    float readTemperature();
    float readPressure();
    float readAltitude(float seaLevelPressure = 1013.25);
    bool logSensorData(const char* filename);

private:
    Adafruit_BMP3XX bmp;
    uint8_t address;
    float temperature;
    float pressure;
    float altitude;
};

#endif
