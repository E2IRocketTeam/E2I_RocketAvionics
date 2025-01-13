#include "MPU9250.h"                      // MPU9250 라이브러리를 포함시킴

MPU9250 mpu;                              // MPU9250 인스턴스를 생성

void setup() {
    Serial.begin(115200);                 // 시리얼 통신을 115200 보드레이트로 초기화
    Wire.begin();                         // I2C 통신을 초기화
    delay(2000);                          // 2초 대기
    
    if (!mpu.setup(0x68)) {               // 주소 0x68로 MPU9250 센서를 설정
        while (1) {                       // 설정 실패 시 무한 루프 진입
            Serial.println("MPU connection failed. Please check your connection with `connection_check` example."); // 오류 메시지 출력
            delay(5000);                  // 5초 대기 후 오류 메시지 반복 출력
        }
    }

    //print_calibration();                // (주석처리됨) 보정 데이터를 출력하는 함수
    mpu.verbose(false);                   // 자세한 출력을 비활성화
}

void loop() {
    if (mpu.update()) {                   // MPU9250 센서 데이터를 업데이트
        static uint32_t prev_ms = millis(); // 이전 millis 값을 저장하는 정적 변수
        if (millis() > prev_ms + 25) {    // 마지막 업데이트 이후 25밀리초가 지났는지 확인
            
            print_rawdata();              // 센서 원시 데이터를 출력
            
            //print_roll_pitch_yaw();      // (주석처리됨) 롤, 피치, 요 데이터를 출력하는 함수
            prev_ms = millis();           // 이전 millis 값을 업데이트
        }
    }
}

void print_roll_pitch_yaw() {
    //Serial.print("Yaw, Pitch, Roll: ");  // (주석처리됨) Yaw, Pitch, Roll 라벨 출력
    Serial.print(mpu.getYaw(), 2);        // 소수점 두 자리까지 Yaw 값 출력
    Serial.print(", ");
    Serial.print(mpu.getPitch(), 2);      // 소수점 두 자리까지 Pitch 값 출력
    Serial.print(", ");
    Serial.println(mpu.getRoll(), 2);     // 소수점 두 자리까지 Roll 값 출력
}

void print_rawdata() {
    Serial.print(mpu.getAccX());          // X축 가속도 출력
    Serial.print(", ");
    Serial.print(mpu.getAccY());          // Y축 가속도 출력
    Serial.print(", ");
    Serial.print(mpu.getAccZ());          // Z축 가속도 출력
    Serial.print(", ");

    Serial.print(mpu.getGyroX());         // X축 자이로스코프 데이터 출력
    Serial.print(", ");
    Serial.print(mpu.getGyroY());         // Y축 자이로스코프 데이터 출력
    Serial.print(", ");
    Serial.print(mpu.getGyroZ());         // Z축 자이로스코프 데이터 출력
    Serial.print(", ");

    Serial.print(mpu.getMagX());          // X축 자기장 데이터 출력
    Serial.print(", ");
    Serial.print(mpu.getMagY());          // Y축 자기장 데이터 출력
    Serial.print(", ");
    Serial.print(mpu.getMagZ());          // Z축 자기장 데이터 출력
    Serial.print(", ");

    Serial.print(mpu.getMagScaleX());     // X축 자기장 스케일 출력
    Serial.print(", ");
    Serial.print(mpu.getMagScaleY());     // Y축 자기장 스케일 출력
    Serial.print(", ");
    Serial.print(mpu.getMagScaleZ());     // Z축 자기장 스케일 출력
    Serial.println("");                   // 개행 문자 출력
}
