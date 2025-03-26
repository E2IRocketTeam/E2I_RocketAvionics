#include "BNO085.h"

BNO085::BNO085(uint8_t address) : address(address) {}

bool BNO085::begin() {
    if (!bno.begin_I2C(address)) {
        return false;
    }

    bno.enableReport(SH2_ACCELEROMETER);
    bno.enableReport(SH2_GYROSCOPE_CALIBRATED);
    bno.enableReport(SH2_MAGNETIC_FIELD_CALIBRATED);
    bno.enableReport(SH2_ROTATION_VECTOR);
    return true;
}

void BNO085::update() {
    sh2_SensorValue_t sensorValue;
    while (bno.getSensorEvent(&sensorValue)) {
        switch (sensorValue.sensorId) {
            case SH2_ACCELEROMETER:
                accelerometer.x = sensorValue.un.accelerometer.x;
                accelerometer.y = sensorValue.un.accelerometer.y;
                accelerometer.z = sensorValue.un.accelerometer.z;
                break;
            case SH2_GYROSCOPE_CALIBRATED:
                gyroscope.x = sensorValue.un.gyroscope.x;
                gyroscope.y = sensorValue.un.gyroscope.y;
                gyroscope.z = sensorValue.un.gyroscope.z;
                break;
            case SH2_MAGNETIC_FIELD_CALIBRATED:
                magnetometer.x = sensorValue.un.magneticField.x;
                magnetometer.y = sensorValue.un.magneticField.y;
                magnetometer.z = sensorValue.un.magneticField.z;
                break;
        }
    }
}

Vector3 BNO085::getAccelerometer() const {
    return accelerometer;
}

Vector3 BNO085::getGyroscope() const {
    return gyroscope;
}

Vector3 BNO085::getMagnetometer() const {
    return magnetometer;
}

void BNO085::readData(float &yaw, float &pitch, float &roll) {
    sh2_SensorValue_t sensorValue;
    if (bno.getSensorEvent(&sensorValue) && sensorValue.sensorId == SH2_ROTATION_VECTOR) {
        float qw = sensorValue.un.rotationVector.real;
        float qx = sensorValue.un.rotationVector.i;
        float qy = sensorValue.un.rotationVector.j;
        float qz = sensorValue.un.rotationVector.k;

        yaw = atan2f(2.0f * (qy * qz + qw * qx), qw * qw - qx * qx - qy * qy + qz * qz) * 180.0f / PI;
        pitch = asinf(-2.0f * (qx * qz - qw * qy)) * 180.0f / PI;
        roll = atan2f(2.0f * (qx * qy + qw * qz), qw * qw + qx * qx - qy * qy - qz * qz) * 180.0f / PI;
    } else {
        yaw = pitch = roll = 0.0f;
    }
}
