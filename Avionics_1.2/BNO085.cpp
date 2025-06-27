#include "BNO085.h"

// 생성자: I2C 주소 초기화
BNO085::BNO085(uint8_t address) : address(address) {}

// 센서 시작 및 리포트 설정
bool BNO085::begin() {
    if (!bno.begin_I2C(address)) {
        return false; // BNO085가 시작되지 않으면 false 반환
    }

    // 회전 벡터와 선형 가속도 리포트를 활성화합니다.
    if (!bno.enableReport(SH2_ROTATION_VECTOR, 5000)) { // 5ms (200Hz)
        return false;
    }
    if (!bno.enableReport(SH2_LINEAR_ACCELERATION, 5000)) { // 5ms (200Hz)
        return false;
    }
    return true; // 성공적으로 초기화되면 true 반환
}

// 센서 데이터 업데이트
void BNO085::update() {
    // 센서 이벤트를 확인하고, 값이 있으면 내부 변수에 저장합니다.
    if (bno.getSensorEvent(&sensorValue)) {
        switch (sensorValue.sensorId) {
            case SH2_ROTATION_VECTOR:
                // 쿼터니언 값을 그대로 sensorValue에 저장해 둡니다.
                // yaw, pitch, roll 변환은 readData()에서 처리합니다.
                break;
            case SH2_LINEAR_ACCELERATION:
                // 가속도 값을 accelerometer 구조체에 저장합니다.
                accelerometer.x = sensorValue.un.linearAcceleration.x;
                accelerometer.y = sensorValue.un.linearAcceleration.y;
                accelerometer.z = sensorValue.un.linearAcceleration.z;
                break;
        }
    }
}

// 저장된 가속도 값을 반환하는 getter 함수
Vector3 BNO085::getAccelerometer() const {
    return accelerometer;
}

// (참고) 자이로스코프 getter 함수 (필요 시 구현)
Vector3 BNO085::getGyroscope() const {
    return gyroscope;
}

// (참고) 지자기 센서 getter 함수 (필요 시 구현)
Vector3 BNO085::getMagnetometer() const {
    return magnetometer;
}


// 저장된 쿼터니언 값을 Yaw, Pitch, Roll로 변환
void BNO085::readData(float &yaw, float &pitch, float &roll) {
    // sensorValue에 저장된 쿼터니언 값을 사용합니다.
    float qx = sensorValue.un.rotationVector.i;
    float qy = sensorValue.un.rotationVector.j;
    float qz = sensorValue.un.rotationVector.k;
    float qw = sensorValue.un.rotationVector.real;

    // Yaw, Pitch, Roll 계산
    float sinr_cosp = 2 * (qw * qx + qy * qz);
    float cosr_cosp = 1 - 2 * (qx * qx + qy * qy);
    roll = atan2(sinr_cosp, cosr_cosp);

    float sinp = 2 * (qw * qy - qz * qx);
    if (abs(sinp) >= 1)
        pitch = copysign(M_PI / 2, sinp);
    else
        pitch = asin(sinp);

    float siny_cosp = 2 * (qw * qz + qx * qy);
    float cosy_cosp = 1 - 2 * (qy * qy + qz * qz);
    yaw = atan2(siny_cosp, cosy_cosp);

    // 라디안을 각도로 변환
    yaw *= 180.0 / M_PI;
    pitch *= 180.0 / M_PI;
    roll *= 180.0 / M_PI;
}