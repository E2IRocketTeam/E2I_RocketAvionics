#ifndef BMP280_H
#define BMP280_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

class BMP280 {
public:
    BMP280(uint8_t address = 0x76); // 기본 I2C 주소는 0x76
    bool begin();
    float readTemperature();
    float readPressure();
    float readAltitude(float seaLevelPressure = 1013.25);

private:
    Adafruit_BMP280 bmp;
    uint8_t address;
};

//이런식으로 사용
//     float temperature = bmp280.readTemperature();
//     float pressure = bmp280.readPressure();
//     float altitude = bmp280.readAltitude(1013.25); // 해수면 기준 기압 (hPa)


#endif
