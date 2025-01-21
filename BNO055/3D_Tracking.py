import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation
import serial
from collections import deque

# Arduino 시리얼 포트 설정
ser = serial.Serial('COM7', 9600, timeout=0.005)  # 'COM3'을 실제 Arduino 포트로 변경하세요.
ser.flush()

# 이동 평균 필터를 위한 버퍼
yaw_buffer = deque(maxlen=5)
pitch_buffer = deque(maxlen=5)
roll_buffer = deque(maxlen=5)

def rotation_matrix(yaw, pitch, roll):
    """
    Create a rotation matrix from yaw, pitch, and roll angles (in degrees).
    """
    yaw, pitch, roll = np.radians([yaw, pitch, roll])

    R_yaw = np.array([
        [np.cos(yaw), -np.sin(yaw), 0],
        [np.sin(yaw),  np.cos(yaw), 0],
        [0, 0, 1]
    ])

    R_pitch = np.array([
        [np.cos(pitch), 0, np.sin(pitch)],
        [0, 1, 0],
        [-np.sin(pitch), 0, np.cos(pitch)]
    ])

    R_roll = np.array([
        [1, 0, 0],
        [0, np.cos(roll), -np.sin(roll)],
        [0, np.sin(roll),  np.cos(roll)]
    ])

    return R_yaw @ R_pitch @ R_roll

def smooth_data(buffer, value):
    """
    Apply a moving average filter to smooth data.
    """
    buffer.append(value)
    return np.mean(buffer)

def update(frame):
    try:
        # 시리얼 데이터를 읽고 파싱
        while ser.in_waiting > 0:  # 시리얼 데이터가 있는 경우에만 처리
            line = ser.readline().decode('utf-8').strip()
            yaw_raw, pitch_raw, roll_raw = map(float, line.split(','))

            # 이동 평균 필터로 데이터 부드럽게 처리
            yaw = smooth_data(yaw_buffer, yaw_raw)
            pitch = smooth_data(pitch_buffer, pitch_raw)
            roll = smooth_data(roll_buffer, roll_raw)

            R = rotation_matrix(yaw, pitch, roll)

            # Rotate the axes
            x_axis_rotated = np.dot(R, x_axis)
            y_axis_rotated = np.dot(R, y_axis)
            z_axis_rotated = np.dot(R, z_axis)

            ax.cla()
            ax.set_xlim([-1, 1])
            ax.set_ylim([-1, 1])
            ax.set_zlim([-1, 1])
            ax.set_xlabel("X")
            ax.set_ylabel("Y")
            ax.set_zlabel("Z")

            # Plot the rotated axes
            ax.quiver(0, 0, 0, *x_axis_rotated, color='r', label='X-axis')
            ax.quiver(0, 0, 0, *y_axis_rotated, color='g', label='Y-axis')
            ax.quiver(0, 0, 0, *z_axis_rotated, color='b', label='Z-axis')

            # Display yaw, pitch, roll values
            ax.text2D(0.05, 0.95, f"Yaw: {yaw:.2f}°\nPitch: {pitch:.2f}°\nRoll: {roll:.2f}°", 
                      transform=ax.transAxes, fontsize=12, color="black")
    except Exception as e:
        print(f"Error: {e}")

# Define the original axes
x_axis = np.array([1, 0, 0])
y_axis = np.array([0, 1, 0])
z_axis = np.array([0, 0, 1])

# Set up the plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ani = FuncAnimation(fig, update, interval=10)  # Update every 10ms

plt.show()
