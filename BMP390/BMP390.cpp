#include "BMP390.h"

BMP390::BMP390() {}

bool BMP390::begin() {
    if (!bmp.begin_I2C()) {
        return false;
    }
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);

    // 초기화: 첫 샘플로 Kalman 필터 초기값 세팅
    bmp.performReading();
    float temp = bmp.temperature;
    float press = bmp.pressure / 100.0;
    float alt = bmp.readAltitude(SEALEVELPRESSURE_HPA);

    tempFilter.setAngle(temp);
    pressFilter.setAngle(press);
    altFilter.setAngle(alt);

    lastTime = millis();
    initialized = true;
    return true;
}

void BMP390::readRawData(float &temperature, float &pressure, float &altitude) {
    if (bmp.performReading()) {
        temperature = bmp.temperature;
        pressure = bmp.pressure / 100.0;
        altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
    }
}

void BMP390::readData(float &temperature, float &pressure, float &altitude) {
    if (bmp.performReading() && initialized) {
        float temp = bmp.temperature;
        float press = bmp.pressure / 100.0;
        float alt = bmp.readAltitude(SEALEVELPRESSURE_HPA);

        unsigned long currentTime = millis();
        float dt = (currentTime - lastTime) / 1000.0;
        lastTime = currentTime;

        temperature = tempFilter.getAngle(temp, 0.0, dt);
        pressure = pressFilter.getAngle(press, 0.0, dt);
        altitude = altFilter.getAngle(alt, 0.0, dt);
    }
}
