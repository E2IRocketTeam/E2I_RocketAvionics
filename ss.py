import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque

# 시리얼 포트 설정
SERIAL_PORT = 'COM6'  # Raspberry Pi에서는 '/dev/ttyUSB0' 등으로 변경
BAUD_RATE = 115200

# 그래프 데이터 저장용 큐 (최근 100개 데이터 저장)
max_length = 100
data = {
    'yaw_raw': deque(maxlen=max_length), 'pitch_raw': deque(maxlen=max_length), 'roll_raw': deque(maxlen=max_length),
    'yaw_filtered': deque(maxlen=max_length), 'pitch_filtered': deque(maxlen=max_length), 'roll_filtered': deque(maxlen=max_length)
}

# 시리얼 포트 열기
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.005)

# 그래프 업데이트 함수
def update(frame):
    if ser.in_waiting:
        try:
            # 시리얼 데이터 읽기
            line = ser.readline().decode('utf-8').strip()
            values = list(map(float, line.split(', ')))  # 데이터 분리 및 변환

            # 데이터 저장 (yaw, pitch, roll 원본 및 필터 적용)
            data['yaw_raw'].append(values[0])
            data['pitch_raw'].append(values[1])
            data['roll_raw'].append(values[2])
            data['yaw_filtered'].append(values[3])
            data['pitch_filtered'].append(values[4])
            data['roll_filtered'].append(values[5])

            # 그래프 업데이트
            ax.clear()
            ax.plot(data['yaw_raw'], label="Yaw (Raw)", color='red', linestyle='solid')
            ax.plot(data['yaw_filtered'], label="Yaw (Filtered)", color='red', linestyle='dashed')

            ax.plot(data['pitch_raw'], label="Pitch (Raw)", color='green', linestyle='solid')
            ax.plot(data['pitch_filtered'], label="Pitch (Filtered)", color='green', linestyle='dashed')

            ax.plot(data['roll_raw'], label="Roll (Raw)", color='blue', linestyle='solid')
            ax.plot(data['roll_filtered'], label="Roll (Filtered)", color='blue', linestyle='dashed')

            ax.legend()
            ax.set_title("Yaw, Pitch, Roll - Raw vs. Filtered")
            ax.set_xlabel("Time")
            ax.set_ylabel("Angle (degrees)")
            ax.set_ylim(-180, 180)
            ax.grid(True)

        except Exception as e:
            print(f"Error: {e}")

# 그래프 초기 설정
fig, ax = plt.subplots(figsize=(8, 5))

ani = animation.FuncAnimation(fig, update, interval=100)
plt.show()

# 종료 시 시리얼 닫기
ser.close()