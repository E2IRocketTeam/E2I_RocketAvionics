#include "BMP280.h"

BMP280::BMP280(uint8_t address) : address(address) {}

bool BMP280::begin() {
    return bmp.begin(address);
}

float BMP280::readTemperature() {
    return bmp.readTemperature();
}

float BMP280::readPressure() {
    return bmp.readPressure();
}

float BMP280::readAltitude(float seaLevelPressure) {
    return bmp.readAltitude(seaLevelPressure);
}

