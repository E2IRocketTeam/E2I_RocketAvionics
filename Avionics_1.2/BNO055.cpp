#include "BNO055.h"

BNO055::BNO055(uint8_t address) : bno(address), address(address) {}

bool BNO055::begin() {
    return bno.begin();
}

void BNO055::setExternalCrystalUse(bool use) {
    bno.setExtCrystalUse(use);
}

void BNO055::readData(float &yaw, float &pitch, float &roll) {
    sensors_event_t orientationData;
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    yaw = orientationData.orientation.x;
    pitch = orientationData.orientation.y;
    roll = orientationData.orientation.z;
}

void BNO055::getEulerAngles(float &yaw, float &pitch, float &roll) {
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    yaw = euler.x();
    pitch = euler.y();
    roll = euler.z();
}



