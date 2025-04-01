#ifndef BMP390_H
#define BMP390_H

#include <Wire.h>
#include <Adafruit_BMP3XX.h>
#include "Kalman.h"

class BMP390 {
public:
    BMP390();
    bool begin();
    void readData(float &temperature, float &pressure, float &altitude);      // 필터 적용된 값
    void readRawData(float &temperature, float &pressure, float &altitude);   // 원본 값

private:
    Adafruit_BMP3XX bmp;
    static constexpr float SEALEVELPRESSURE_HPA = 1013.25;

    Kalman tempFilter;
    Kalman pressFilter;
    Kalman altFilter;
    unsigned long lastTime = 0;
    bool initialized = false;
};

#endif
