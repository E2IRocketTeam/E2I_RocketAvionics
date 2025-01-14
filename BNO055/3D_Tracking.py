import serial
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation

# 시리얼 포트 설정 (IMU와 연결)
SERIAL_PORT = 'COM3'  # IMU 연결 포트
BAUD_RATE = 115200

# 시리얼 통신 초기화
ser = serial.Serial(SERIAL_PORT, BAUD_RATE)

# 3D 시각화 초기화
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.set_xlim([-1, 1])
ax.set_ylim([-1, 1])
ax.set_zlim([-1, 1])

# 축 초기화
vector_line, = ax.plot([0, 1], [0, 0], [0, 0], 'r', label="X-axis")
ax.quiver(0, 0, 0, 1, 0, 0, color='r', label='Orientation')

# 데이터 업데이트 함수
def update(frame):
    # 시리얼 데이터 읽기
    line = ser.readline().decode('utf-8').strip()
    try:
        # 오일러 각도(Yaw, Pitch, Roll) 파싱
        yaw, pitch, roll = map(float, line.split(','))  # "Yaw, Pitch, Roll" 형식
        yaw, pitch, roll = np.radians([yaw, pitch, roll])  # 각도를 라디안으로 변환

        # 회전 행렬 계산
        R_x = np.array([
            [1, 0, 0],
            [0, np.cos(roll), -np.sin(roll)],
            [0, np.sin(roll), np.cos(roll)]
        ])
        R_y = np.array([
            [np.cos(pitch), 0, np.sin(pitch)],
            [0, 1, 0],
            [-np.sin(pitch), 0, np.cos(pitch)]
        ])
        R_z = np.array([
            [np.cos(yaw), -np.sin(yaw), 0],
            [np.sin(yaw), np.cos(yaw), 0],
            [0, 0, 1]
        ])
        R = R_z @ R_y @ R_x  # 전체 회전 행렬

        # 새 방향 벡터 계산
        x_dir = R @ np.array([1, 0, 0])
        vector_line.set_data([0, x_dir[0]], [0, x_dir[1]])
        vector_line.set_3d_properties([0, x_dir[2]])

    except ValueError:
        print("잘못된 데이터: ", line)

    return vector_line,

# 애니메이션 시작
ani = FuncAnimation(fig, update, interval=50)
plt.legend()
plt.show()
