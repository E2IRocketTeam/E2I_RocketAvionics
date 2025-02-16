#include "LoRaCommandSystem.h"

LoRaCommandSystem::LoRaCommandSystem(int parachutePin) 
    : parachute(parachutePin) {}

void LoRaCommandSystem::begin() {
    parachute.begin();
    Serial.begin(9600);
    LORA.begin(9600);
    LORA.println("AT+ADDRESS=2");
    delay(500);
}

void LoRaCommandSystem::processCommands() {
    if (LORA.available()) {
        String receivedData = LORA.readString();

        if (receivedData == PARACHUTE_OPEN_KEY) {
            parachute.openParachute();
        } else if (receivedData == PARACHUTE_CLOSE_KEY) {
            parachute.closeParachute();
        } else if (receivedData == RESET_KEY) {
            softwareReset();
        }
    }
}

void LoRaCommandSystem::softwareReset() {
    SCB_AIRCR = 0x05FA0004 | (1 << 2); // System Reset Request (SYSRESETREQ)
}
