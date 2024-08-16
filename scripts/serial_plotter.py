import serial
import time
import matplotlib.pyplot as plt
import argparse


class SerialPlotter:
    def __init__(self, port, baudrate=9600, timeout=0.01, max_data_points=160):
        self.serial = serial.Serial(port=port, baudrate=baudrate, timeout=timeout)
        time.sleep(1)  # Allow time for the connection to establish

        self.x_data = []
        self.y_data = []
        self.max_data_points = max_data_points

        plt.ion()  # Turn on interactive mode for live updating

    def update_plot(self):
        plt.clf()  # Clear the previous plot
        plt.plot(self.x_data, self.y_data, "r.")
        plt.xlabel("X [cm]")
        plt.ylabel("Y [cm]")
        plt.title("Plot of Y(X)")
        plt.draw()
        plt.pause(0.001)

    def start(self):
        try:
            while True:
                line = self.serial.readline()
                if line:
                    line_str = line.decode()
                    coordinates = line_str.strip().split(", ")
                    
                    if len(coordinates) == 2:  # Ensure we have both X and Y
                        try:
                            x = float(coordinates[0])
                            y = float(coordinates[1])
                            if len(self.x_data) > self.max_data_points:
                                self.x_data.pop(0)
                            if len(self.y_data) > self.max_data_points:
                                self.y_data.pop(0)
                            self.x_data.append(x)
                            self.y_data.append(y)
                            self.update_plot()
                        except ValueError:
                            print("Invalid data format. Could not convert to float.")
        except KeyboardInterrupt:
            self.close()
            plt.ioff()

    def close(self):
        self.serial.close()
        print("Serial connection closed.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Serial Plotter for live data plotting."
    )
    parser.add_argument("port", help="The serial port to connect to (e.g., COM8).")
    parser.add_argument(
        "--baudrate",
        type=int,
        default=9600,
        help="Baud rate for serial communication (default: 9600).",
    )
    parser.add_argument(
        "--timeout",
        type=float,
        default=0.01,
        help="Read timeout for serial communication in seconds (default: 0.01).",
    )
    parser.add_argument(
        "--max_data_points",
        type=int,
        default=160,
        help="Maximum number of data points to display (default: 160).",
    )

    args = parser.parse_args()

    plotter = SerialPlotter(
        port=args.port,
        baudrate=args.baudrate,
        timeout=args.timeout,
        max_data_points=args.max_data_points,
    )
    plotter.start()
