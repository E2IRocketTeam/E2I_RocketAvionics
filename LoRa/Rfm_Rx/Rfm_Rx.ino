// =======================================================================
//  Avionics_1.2.ino (송신기 - 로켓) 수정 코드
// =======================================================================
// E2I Rocket Avionics Main Control Code (v1.5 - Binary Transfer)
#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>

#include "Parachute.h"
#include "Sdcard.h"
#include "GPS.h"

// --- LoRa 핀 설정 및 주파수 ---
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 915.0

const int SERVO_PIN = 22;

// --- 전역 객체 생성 ---
RH_RF95 rf95(RFM95_CS, RFM95_INT);
Parachute parachute(SERVO_PIN);
GPS gps(Serial1);

const char* logFilename = "flight_log.csv";
unsigned long programStartTime = 0;

// --- 데이터 패킷 구조체 정의 ---
// LoRa로 전송할 데이터들을 하나의 묶음(구조체)으로 정의합니다.
struct DataPacket {
    float yaw, pitch, roll;
    float accX, accY, accZ;
    float temperature, pressure, altitude;
    double latitude, longitude, gpsAlt;
    int sats;
};


void setup() {
    Serial.begin(115200);
    while (!Serial);
    Serial.println("===== Avionics System Start (v1.5 - Binary Transfer) =====");

    parachute.begin();
    initializeSD();
    createLogFile(logFilename);

    // LoRa 초기화
    if (!rf95.init()) {
        Serial.println("LoRa radio init failed");
        while (1);
    }
    rf95.setFrequency(RF95_FREQ);
    rf95.setTxPower(23, false);
    
    // (선택) 더 빠른 통신 모드 설정 (거리는 약간 짧아질 수 있음)
    // rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128);

    gps.begin(9600);
    Serial.println("GPS initialized");

    programStartTime = millis();
    Serial.println("Sensor stabilization period started (5 seconds)...");
}

void loop() {
    parachute.update();
    gps.update();

    if (millis() - programStartTime < 5000) {
        Serial.print(".");
        delay(500);
        return;
    }

    // --- 데이터 수집 ---
    DataPacket packet; // 데이터 패킷 인스턴스 생성
    float temp_yaw, temp_pitch, temp_roll;
    Vector3 temp_acc;

    parachute.getOrientation(temp_yaw, temp_pitch, temp_roll);
    packet.yaw = temp_yaw;
    packet.pitch = temp_pitch;
    packet.roll = temp_roll;
    
    temp_acc = parachute.getAcceleration();
    packet.accX = temp_acc.x;
    packet.accY = temp_acc.y;
    packet.accZ = temp_acc.z;

    packet.temperature = parachute.getTemperature();
    packet.pressure = parachute.getPressure();
    packet.altitude = parachute.getAltitude();

    packet.latitude = gps.getLatitude();
    packet.longitude = gps.getLongitude();
    packet.gpsAlt = gps.getAltitude();
    packet.sats = gps.getSatellites();

    // --- SD 카드에 기록 (기존과 동일) ---
    logData(logFilename, packet.yaw, packet.pitch, packet.roll,
            packet.accX, packet.accY, packet.accZ,
            packet.temperature, packet.pressure, packet.altitude,
            packet.latitude, packet.longitude, packet.gpsAlt, packet.sats);

    // --- 시리얼 출력 (기존과 동일) ---
    // (생략 - 필요시 기존 코드 유지)


    // --- LoRa 무선 전송 (수정된 부분) ---
    // snprintf로 긴 문자열을 만드는 대신, 구조체 자체를 바이트 형태로 전송합니다.
    // 훨씬 빠르고 효율적입니다.
    rf95.send((uint8_t*)&packet, sizeof(packet));
    rf95.waitPacketSent();

    delay(10);
}
```cpp
// =======================================================================
//  Rfm_Rx.ino (수신기 - 지상국) 수정 코드
// =======================================================================
// E2I 지상국(Ground Station) 수신 코드 (v1.5 - Binary Transfer)
#include <SPI.h>
#include <RH_RF95.h>

// --- LoRa 핀 설정 및 주파수 (송신기와 동일하게 설정) ---
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 915.0

// --- LoRa 객체 생성 ---
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// --- 데이터 패킷 구조체 정의 ---
// 송신기에서 보낸 묶음과 정확히 동일한 형태로 정의해야 합니다.
struct DataPacket {
    float yaw, pitch, roll;
    float accX, accY, accZ;
    float temperature, pressure, altitude;
    double latitude, longitude, gpsAlt;
    int sats;
};

void setup() {
    Serial.begin(115200);
    while (!Serial);
    
    Serial.println("RFM95 LoRa 수신기 초기화 시작 (Binary Mode)...");

    if (!rf95.init()) {
        Serial.println("LoRa 수신기 초기화 실패!");
        while (1);
    }
    Serial.println("LoRa 수신기 초기화 성공!");

    if (!rf95.setFrequency(RF95_FREQ)) {
        Serial.println("주파수 설정 실패!");
        while (1);
    }
    Serial.print("주파수 설정 완료: "); Serial.println(RF95_FREQ);
    
    // (선택) 송신기와 동일하게 빠른 통신 모드 설정
    // rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128);
    
    Serial.println("수신 대기 중...");
}

void loop() {
    if (rf95.available()) {
        // 데이터 패킷을 담을 변수 생성
        DataPacket packet;
        
        // 수신된 데이터의 길이를 확인
        uint8_t len = sizeof(packet);
        
        // sscanf로 파싱하는 대신, 수신된 바이트들을 구조체에 바로 복사합니다.
        if (rf95.recv((uint8_t*)&packet, &len)) {
            
            // 수신된 데이터의 길이가 예상과 정확히 일치하는지 확인
            if (len == sizeof(packet)) {
                Serial.print(packet.yaw); Serial.print(", "); Serial.print(packet.pitch); Serial.print(", "); Serial.println(packet.roll);
                Serial.print(packet.accX); Serial.print(", "); Serial.print(packet.accY); Serial.print(", "); Serial.println(packet.accZ);
                Serial.print(packet.temperature); Serial.print(" C, "); Serial.print(packet.pressure); Serial.print(" hPa, "); Serial.print(packet.altitude); Serial.println(" m");
                Serial.print(packet.latitude, 6); Serial.print(", "); Serial.print(packet.longitude, 6); Serial.print(", "); Serial.println(packet.gpsAlt, 2);
                Serial.print(packet.sats); Serial.print("개, RSSI: "); Serial.println(rf95.lastRssi(), DEC);
                Serial.println("---------------------------");
            } else {
                Serial.println("⚠️ 수신된 데이터 길이가 일치하지 않습니다!");
            }
        }
    }
}
