# Distance Scanner

## Project Goal
The goal of this project is to create a distance sensor that rotates and, based on its measurements, draws a y(x) plot of the environment around the sensor. It should work like a scanner for the environment.

## Prerequisites
**Electronic Components:**
* Arduino UNO board
* 28BYJ-48 stepper motor
* ULN2003 stepper motor driver
* HC-SR04 distance sensor

**Other Components:**
* Breadboard
* Push button
* USB to Arduino cable
* Breadboard to Arduino and other component cables
* Stepper motor mount on which you can attach the distance sensor, e.g., [print it with a 3D printer](https://www.thingiverse.com/thing:2999350).

## System Composition
The image below shows the system composition.

<img src="images/20230919_102026.jpg" alt="System Composition" width="400" class="center"/>

The Arduino board should be connected to the PC, and the stepper motor should be connected to the driver. The stepper motor mount should be placed on the stepper motor with the distance sensor on top of it. The pins where components should be connected to the Arduino can be configured in the [code](arduino/arduino.ino).

## Usage
1. Connect the Arduino board to the PC.
2. Run the Python script to plot the results:
```
python3 serial_plotter.py <COM> --baudrate <baudrate> --timeout <timeout> --max_data_points <max_data_points>
```
Where:
* **port**: The serial port is required as a positional argument.
* **--baudrate**: An optional argument to specify the baud rate, with a default of 9600.
* **--timeout**: An optional argument for the read timeout, with a default of 0.01 seconds.
* **--max_data_points**: An optional argument to set the maximum number of data points to display on the plot, with a default of 160.
3. Now program is in the config mode. Press the push button when the sensor is at the point where you want the center of the scan to be. It starts plotting (leaves config mode).
4. You can now control the program by sending commands through the Serial Monitor:
* _reset_: Resets the center of rotation.
* _stop_: Stops the motion of the stepper motor.
* _start_: Starts the motion of the stepper motor.
* _rotate {steps}_: Manually moves the stepper motor by the specified number of steps.
