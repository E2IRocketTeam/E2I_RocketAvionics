#include <Wire.h>
#include <Adafruit_BMP3XX.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include "Sdcard.h"

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
const char* filename = "sensor_data.csv";

void setup() {
    Serial.begin(115200);
    while (!Serial);

    if (!initializeSD()) {
        Serial.println("SD card initialization failed!");
        while (1);
    }
    Serial.println("SD card initialized.");

    if (!bmp.begin_I2C()) {
        Serial.println("Could not find a valid BMP390 sensor, check wiring!");
        while (1);
    }
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);

    if (!bno.begin()) {
        Serial.println("No BNO055 detected, check wiring!");
        while (1);
    }
    bno.setExtCrystalUse(true);

    if (!createLogFile(filename)) {
        Serial.println("Failed to create log file!");
        while (1);
    }
}

void loop() {
    if (!bmp.performReading()) {
        Serial.println("Failed to read BMP390 sensor");
        return;
    }
    sensors_event_t orientationData, accelData;
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&accelData, Adafruit_BNO055::VECTOR_ACCELEROMETER);

    float yaw = orientationData.orientation.x;
    float pitch = orientationData.orientation.y;
    float roll = orientationData.orientation.z;
    float temperature = bmp.temperature;
    float pressure = bmp.pressure / 100.0;
    float altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);

    if (logData(filename, yaw, pitch, roll, temperature, pressure, altitude)) {
        Serial.print("Logged Data: ");
        Serial.print(yaw, 2); Serial.print(", ");
        Serial.print(pitch, 2); Serial.print(", ");
        Serial.print(roll, 2); Serial.print(", ");
        Serial.print(temperature, 2); Serial.print(", ");
        Serial.print(pressure, 2); Serial.print(", ");
        Serial.println(altitude, 2);
    } else {
        Serial.println("Failed to log data");
    }
    
}
