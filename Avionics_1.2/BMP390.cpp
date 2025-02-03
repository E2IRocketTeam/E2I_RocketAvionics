#include "BMP390.h"

BMP390::BMP390() {}

bool BMP390::begin() {
    if (!bmp.begin_I2C()) {
        return false;
    }
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    return true;
}

void BMP390::readData(float &temperature, float &pressure, float &altitude) {
    if (bmp.performReading()) {
        temperature = bmp.temperature;
        pressure = bmp.pressure / 100.0;
        altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
    }
}
