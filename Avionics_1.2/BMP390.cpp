#include "BMP390.h"

BMP390::BMP390(uint8_t addr) : address(addr), temperature(0.0), pressure(0.0), altitude(0.0) {}

bool BMP390::begin() {
    if (!bmp.begin_I2C(address)) {
        return false;
    }
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    return true;
}

void BMP390::readSensorData() {
    if (bmp.performReading()) {
        temperature = bmp.temperature;
        pressure = bmp.pressure / 100.0;
        altitude = bmp.readAltitude(1013.25);
    }
}

float BMP390::readTemperature() {
    return temperature;
}

float BMP390::readPressure() {
    return pressure;
}

float BMP390::readAltitude(float seaLevelPressure) {
    return bmp.readAltitude(seaLevelPressure);
}

bool BMP390::logSensorData(const char* filename) {
    return logData(filename, 0.0, 0.0, 0.0, temperature, pressure, altitude);
}
