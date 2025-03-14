import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import time

# Set up serial connection (update 'COM3' to your port)
ser = serial.Serial('COM16', 115200, timeout=0.005)  # Replace 'COM3' with your port
time.sleep(2)  # Allow some time for the connection to stabilize

# Data storage
bno085_pitch = []
bno055_pitch = []
timestamps = []

# Plotting setup
fig, ax = plt.subplots(figsize=(10, 6))
ax.set_title("Pitch Comparison: BNO085 vs BNO055")
ax.set_xlabel("Time (samples)")
ax.set_ylabel("Pitch (degrees)")
ax.set_xlim(0, 100)
ax.set_ylim(-180, 180)
line_085, = ax.plot([], [], label="BNO085 Pitch", color="blue")
line_055, = ax.plot([], [], label="BNO055 Pitch", color="red")
ax.legend()

def update(frame):
    global timestamps, bno085_pitch, bno055_pitch

    try:
        # Read line from serial
        line = ser.readline().decode('utf-8').strip()
        if line:
            # Parse pitch values
            pitch_values = line.split(",")
            if len(pitch_values) == 2:
                pitch_085 = float(pitch_values[0])
                pitch_055 = float(pitch_values[1])

                # Append to data lists
                bno085_pitch.append(pitch_085)
                bno055_pitch.append(pitch_055)
                timestamps.append(len(timestamps))

                # Keep only the latest 100 samples
                if len(timestamps) > 100:
                    bno085_pitch.pop(0)
                    bno055_pitch.pop(0)
                    timestamps.pop(0)

    except Exception as e:
        print(f"Error: {e}")

    # Update plot data
    line_085.set_data(timestamps, bno085_pitch)
    line_055.set_data(timestamps, bno055_pitch)

    # Adjust axes limits dynamically
    ax.set_xlim(max(0, len(timestamps) - 100), len(timestamps))

    return line_085, line_055

# Create animation
ani = FuncAnimation(fig, update, interval=100)
plt.tight_layout()
plt.show()

# Close serial connection on exit
ser.close()
