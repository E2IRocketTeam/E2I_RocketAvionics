import serial
import time

# 시리얼 포트 및 보드레이트 설정
serial_port = 'COM6'  # Windows: 'COM3', Mac/Linux: '/dev/ttyUSB0'
baud_rate = 9600

try:
    ser = serial.Serial(serial_port, baud_rate, timeout=1)
    time.sleep(2)  # 포트 안정화 대기

    print("시리얼 데이터 수신 시작...")
    while True:
        # 아두이노에서 데이터 수신
        line = ser.readline().decode('utf-8').strip()
        if line:
            try:
                # 데이터를 파싱
                yaw, pitch, roll, temp = map(float, line.split(","))
                print(f"Yaw: {yaw:.2f}, Pitch: {pitch:.2f}, Roll: {roll:.2f}, 온도: {temp:.2f}°C")
            except ValueError:
                print("잘못된 데이터 수신:", line)

except serial.SerialException as e:
    print(f"시리얼 포트를 열 수 없습니다: {e}")
except KeyboardInterrupt:
    print("\n프로그램 종료")
finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
