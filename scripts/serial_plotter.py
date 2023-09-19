import serial
import time
import matplotlib.pyplot as plt

ser = serial.Serial(port='COM5', baudrate=9600, timeout=0.01)
time.sleep(1)

x_data = []
y_data = []

max_data_points = 160

plt.ion()  # Turn on interactive mode for live updating

try:
    while True:
        line = ser.readline()
        if line:
            line_str = line.decode()
            print(line_str)
            coordinates = line_str.split(", ")
            x = float(coordinates[0])
            y = float(coordinates[1])

            if len(x_data) > max_data_points:
                x_data.pop(0)

            if len(y_data) > max_data_points:
                y_data.pop(0)

            x_data.append(x)
            y_data.append(y)

            plt.clf()  # Clear the previous plot
            plt.plot(x_data, y_data, "r.")
            plt.xlabel('X [cm]')
            plt.ylabel('Y [cm]')
            plt.title('Plot of Y(X)')
            plt.draw()
            plt.pause(0.001)
except KeyboardInterrupt:
    ser.close()
    plt.ioff()
