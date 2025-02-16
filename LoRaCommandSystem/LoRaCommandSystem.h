#ifndef LORA_COMMAND_SYSTEM_H
#define LORA_COMMAND_SYSTEM_H

#include "Parachute.h"

#define LORA Serial2 // Serial2 사용 (TX=8, RX=7)

#define PARACHUTE_OPEN_KEY "OP"
#define PARACHUTE_CLOSE_KEY "CP"
#define RESET_KEY "R"

class LoRaCommandSystem {
public:
    LoRaCommandSystem(int parachutePin);
    void begin();
    void processCommands();
    void softwareReset();

private:
    Parachute parachute;
};

#endif // LORA_COMMAND_SYSTEM_H
