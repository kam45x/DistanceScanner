#include <Stepper.h>

// Distance sensor pins
#define trigPin 4
#define echoPin 3
// Stepper pins
#define stepperIN1Pin 8
#define stepperIN2Pin 9
#define stepperIN3Pin 10
#define stepperIN4Pin 11
// Configuration button pin
#define buttonPin 2

// Defines the number of steps per rotation
const int stepsPerRevolution = 2038;

// Information about rotation
const int max_position = 400;
const int single_step = 10;

// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, stepperIN1Pin, stepperIN3Pin, stepperIN2Pin, stepperIN4Pin);

int current_position = 0;
bool positive_direction = true;

// User control variables
String input_data = "";
bool run = true;
bool config_mode = true;

// Max and min distance from distance sensor documentation in centimeters
float min_distance = 2;
float max_distance = 200;

void setup() 
{
  // Stepper Library sets pins as outputs
	myStepper.setSpeed(5);

	Serial.begin(9600);
	pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() 
{
  read_serial_input();
  read_config_button();

  if (run)
  {
    if (!config_mode)
    {
      bool valid_distance = true;
      float distance = calculate_distance();
      // Check if sensor really allow to measure such distance
      if (distance < min_distance || distance > max_distance)
      {
        valid_distance = false;
      }

      if (valid_distance)
      {
        float angle = (static_cast<float>(current_position) / stepsPerRevolution) * 2 * M_PI;
        float x = distance * sin(angle);
        float y = distance * cos(angle);

        Serial.print(x);
        Serial.print(", ");
        Serial.println(y);
      }
    }
    
    move_stepper();
    delay(10);
  }
}

float calculate_distance()
{
  long time;
  float distance;

  // Wait for impulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  time = pulseIn(echoPin, HIGH);
  // Convert
  float time_float = time;
  distance = time_float / 58;

  return distance;
}

void move_stepper()
{
  if (positive_direction)
  {
    if (current_position <= max_position - single_step)
    {
      myStepper.step(single_step);
	    current_position = current_position + single_step;
    }
    else
    {
      positive_direction = false;
      // Delay before direction change
      delay(100);
    }
  }
  else if (!positive_direction)
  {
    if (current_position >= -max_position + single_step)
    {
      myStepper.step(-single_step);
      current_position = current_position - single_step;
    }
    else
    {
      positive_direction = true;
      // Delay before direction change
      delay(100);
    }
  }
}

void read_serial_input()
{
 if (Serial.available() > 0)
 {
  input_data = Serial.readStringUntil('\n');

  if (input_data == "start" || input_data == "Start")
  {
    run = true;
  }
  else if (input_data == "stop" || input_data == "Stop")
  {
    run = false;
  }
  else if (input_data.startsWith("rotate") || input_data.startsWith("Rotate")
           && !run)
  {
    int angle = input_data.substring(6).toInt();
    myStepper.step(angle);
  }
  else if (input_data == "reset" || input_data == "Reset" && !run)
  {
    reset();
  }
  else
  {
    Serial.println("Invalid command!");
  }
 }
}

void reset()
{
  current_position = 0;
  positive_direction = true;
}

void read_config_button()
{
  if (config_mode && digitalRead(buttonPin) == LOW)
  {
    config_mode = false;
    reset();
    delay(2000);
  }
  else if (!config_mode && digitalRead(buttonPin) == LOW)
  {
    config_mode = true;
    reset();
    delay(1000);
  }
}
