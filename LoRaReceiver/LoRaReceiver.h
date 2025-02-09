#ifndef LORA_RECEIVER_H
#define LORA_RECEIVER_H

#include <SPI.h>
#include <RH_RF95.h>

class LoRaReceiver {
public:
    LoRaReceiver(int csPin, int rstPin, int intPin, float frequency);
    bool begin();
    bool receiveData(char* receivedMessage, uint8_t maxLength, int &rssi);

private:
    int csPin;
    int rstPin;
    int intPin;
    float frequency;
    RH_RF95 rf95;
};

#endif // LORA_RECEIVER_H
