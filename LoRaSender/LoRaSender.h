#ifndef LORA_SENDER_H
#define LORA_SENDER_H

#include <SPI.h>
#include <RH_RF95.h>

class LoRaSender {
public:
    LoRaSender(int csPin, int rstPin, int intPin, float frequency);
    bool begin();
    void sendData(float yaw, float pitch, float roll, float temperature, float pressure, float altitude);

private:
    int csPin;
    int rstPin;
    int intPin;
    float frequency;
    RH_RF95 rf95;
};

#endif // LORA_SENDER_H
