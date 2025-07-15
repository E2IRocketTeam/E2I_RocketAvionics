void setup() {
  // USB 디버깅용 시리얼 (9600 baud)
  Serial.begin(9600);
  while (!Serial); // USB 시리얼 연결 대기 (필요시 사용)
  
  // RYLR896과의 통신용 하드웨어 시리얼 (예: Serial1)
  Serial1.begin(9600);
  delay(1000);
  
  Serial.println("Teensy Transmitter - RYLR896 송신 모드 시작");

  // RYLR896 초기 설정
  Serial1.println("AT");                 // 모듈 응답 확인
  delay(500);
  Serial1.println("AT+ADDRESS=1");         // 송신기 주소 설정 (1)
  delay(500);
  Serial1.println("AT+NETWORKID=5");       // 네트워크 ID 설정 (송신/수신 일치)
  delay(500);
  Serial1.println("AT+BAND=862000000");    // 주파수 설정 (예: 915MHz)
  delay(500);

  Serial.println("입력할 메시지를 보내면 송신됩니다.");
}

void loop() {
  if (Serial.available()) {
    String userInput = Serial.readStringUntil('\n'); // 엔터 칠 때까지 입력
    userInput.trim(); // 공백, \r 제거

    if (userInput.length() > 0) {
      // 송신 명령 구성 및 전송
      Serial1.print("AT+SEND=2,"); // 수신기 주소가 2
      Serial1.print(userInput.length());
      Serial1.print(",");
      Serial1.println(userInput);

      // USB 시리얼에도 로그 출력
      Serial.print("송신: ");
      Serial.println(userInput);
    }
  }
}
