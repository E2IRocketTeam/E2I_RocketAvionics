#ifndef LORA_COMMAND_SYSTEM_H
#define LORA_COMMAND_SYSTEM_H

#include <Arduino.h>
#include "ForcedParachute.h"

// 명령어 정의
#define PARACHUTE_OPEN_KEY "OP"
#define PARACHUTE_CLOSE_KEY "CP"
#define RESET_KEY "R"

class LoRaCommandSystem {
public:
    LoRaCommandSystem(Stream &loraStream, ForcedParachute &parachute);
    void begin();
    void processCommands();

private:
    void softwareReset();

    Stream &loraStream;         // LoRa 통신을 위한 Stream 객체 참조
    ForcedParachute &parachute; // ForcedParachute 객체 참조
};

#endif
