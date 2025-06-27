#ifndef BNO085_H
#define BNO085_H

#include <Wire.h>
#include <Adafruit_BNO08x.h>

// 3축 벡터 데이터(가속도, 자이로, 지자기)를 담기 위한 구조체
struct Vector3 {
    float x;
    float y;
    float z;
};

class BNO085 {
public:
    // 생성자
    BNO085(uint8_t address = 0x4A);
    // 센서 초기화
    bool begin();
    // 센서의 최신 값을 내부 변수로 읽어오는 함수
    void update();

    // 내부 변수에 저장된 가속도, 자이로, 지자기 값을 반환하는 함수
    Vector3 getAccelerometer() const;
    Vector3 getGyroscope() const;     // (참고) 현재 구현에서는 사용되지 않음
    Vector3 getMagnetometer() const;  // (참고) 현재 구현에서는 사용되지 않음

    // 내부 변수의 회전 벡터 값을 Yaw, Pitch, Roll로 변환하여 반환하는 함수
    void readData(float &yaw, float &pitch, float &roll);

private:
    // Adafruit 라이브러리 객체
    Adafruit_BNO08x bno;
    // I2C 주소
    uint8_t address;

    // 센서 값을 저장하기 위한 내부 변수
    Vector3 accelerometer;
    Vector3 gyroscope;
    Vector3 magnetometer;
    // 쿼터니언 값을 저장하기 위한 변수
    sh2_SensorValue_t sensorValue;
};

#endif