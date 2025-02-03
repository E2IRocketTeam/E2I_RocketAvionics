#include "BNO055.h"

BNO055::BNO055() : bno(55, 0x28, &Wire) {}

bool BNO055::begin() {
    if (!bno.begin()) {
        return false;
    }
    bno.setExtCrystalUse(true);
    return true;
}

void BNO055::readData(float &yaw, float &pitch, float &roll) {
    sensors_event_t orientationData;
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    yaw = orientationData.orientation.x;
    pitch = orientationData.orientation.y;
    roll = orientationData.orientation.z;
}
