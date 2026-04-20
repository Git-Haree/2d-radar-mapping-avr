import serial
import matplotlib.pyplot as plt
import numpy as np
import math

# Configure serial port based on system
SERIAL_PORT = 'COM3'  # Change to /dev/ttyUSB0 on Linux
BAUD_RATE = 115200

def main():
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.1)
    except Exception as e:
        print(f"Error opening serial port: {e}")
        return

    plt.ion()
    fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
    ax.set_ylim(0, 200) # Max 200cm
    ax.set_theta_zero_location("N")
    ax.set_theta_direction(-1)
    
    angles = []
    distances = []
    
    scatter = ax.scatter(angles, distances, c='r', s=10)
    print("Starting Radar Perception Mapping... (Press Ctrl+C to stop)")

    try:
        while True:
            line = ser.readline().decode('utf-8').strip()
            if line:
                try:
                    angle_deg, dist_cm = map(int, line.split(','))
                    # Convert to radians for polar plot
                    angle_rad = math.radians(angle_deg)
                    
                    # Store data points
                    angles.append(angle_rad)
                    distances.append(dist_cm)
                    
                    # Keep only last 180 points for a sweep effect
                    if len(angles) > 180:
                        angles.pop(0)
                        distances.pop(0)
                        
                    scatter.set_offsets(np.c_[angles, distances])
                    fig.canvas.flush_events()
                except ValueError:
                    pass
    except KeyboardInterrupt:
        print("Stopping radar.")
        ser.close()

if __name__ == "__main__":
    main()
