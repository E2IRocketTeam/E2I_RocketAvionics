
#define LORA Serial2  // Serial2 사용 (TX=8, RX=7)

#define PARACHUTE_OPEN_KEY "OP"
#define PARACHUTE_CLOSE_KEY "CP"
#define RESET_KEY "R"

void setup() {
    parachute.begin();
    Serial.begin(9600);
    LORA.begin(9600);
    LORA.println("AT+ADDRESS=2");
    delay(500);
}
void softwareReset() {
    SCB_AIRCR = 0x05FA0004 | (1 << 2); // System Reset Request (SYSRESETREQ)
}

void loop() {
    if (LORA.available()) {
        String receivedData = LORA.readString();
        
        if(receivedData ==  PARACHUTE_OPEN_KEY) {
            //parachute.openParachute();
        }
        else if(receivedData ==  PARACHUTE_CLOSE_KEY) {
            //parachute.closeParachute();
        }
        else if(receivedData ==  RESET_KEY) {
            softwareReset(); 
        }
        
    }
}