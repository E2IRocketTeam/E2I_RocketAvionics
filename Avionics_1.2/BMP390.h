#ifndef BMP390_H
#define BMP390_H

#include <Wire.h>
#include <Adafruit_BMP3XX.h>

class BMP390 {
public:
    BMP390();
    bool begin();
    void readData(float &temperature, float &pressure, float &altitude);

private:
    Adafruit_BMP3XX bmp;
    static constexpr float SEALEVELPRESSURE_HPA = 1013.25;
};

#endif
