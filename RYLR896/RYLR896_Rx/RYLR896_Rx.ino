void setup() {
    // USB 시리얼 (디버깅용)
    while (!Serial); // USB 시리얼 연결 대기 (필요시 사용)
    
    // RYLR896과의 통신을 위한 하드웨어 시리얼 (예: Serial1 사용)
    Serial1.begin(115200);  
    delay(1000);
    
    Serial.println("RYLR896 수신기 모드 시작...");
  
    // RYLR896 모듈 초기 설정 (필요에 따라 수정)
    Serial1.println("AT");                // 모듈 응답 확인
    delay(500);
    Serial1.println("AT+ADDRESS=2");        // 수신기 주소 설정 (송신기와 맞춰야 함)
    delay(500);
    Serial1.println("AT+NETWORKID=5");      // 네트워크 ID 설정 (송신기와 동일)
    delay(500);
    Serial1.println("AT+BAND=868000000");   // 주파수 설정 (예: 868MHz)
    delay(500);
  
    // 필요시 수신 모드 명령 추가 (모듈 매뉴얼 참조)
    // 예: Serial1.println("AT+RX");
  }
  
  void loop() {
    // RYLR896 모듈에서 데이터가 들어왔는지 확인
    if (Serial1.available()) {
      // 개행 문자('\n')까지 읽어서 문자열 생성
      String receivedMessage = Serial1.readStringUntil('\n');
      Serial.print("수신: ");
      Serial.println(receivedMessage);
    }
  }
  