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

// Defines the number of steps per revolution
const int stepsPerRevolution = 2038;

// Max and min distance from distance sensor documentation in centimeters
float minDistance = 2;
float maxDistance = 200;

// Information about rotation
const int maxPosition = 400;
const int singleStep = 10;

// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, stepperIN1Pin, stepperIN3Pin, stepperIN2Pin, stepperIN4Pin);

int currentPosition = 0;
bool positiveDirection = true;

// User control variables
String inputData = "";
bool run = true;
bool configMode = true;

void setup()
{
  myStepper.setSpeed(5);  // Stepper Library sets pins as outputs

  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop()
{
  readSerialInput();
  readConfigButton();

  if (run)
  {
    if (!configMode)
    {
      bool validDistance = true;
      float distance = calculateDistance();

      // Check if sensor really allow to measure such distance
      if (distance < minDistance || distance > maxDistance)
      {
        validDistance = false;
      }

      if (validDistance)
      {
        float angle = (static_cast<float>(currentPosition) / stepsPerRevolution) * 2 * M_PI;
        float x = distance * sin(angle);
        float y = distance * cos(angle);

        Serial.print(x);
        Serial.print(", ");
        Serial.println(y);
      }
    }

    moveStepper();
    delay(10);
  }
}

float calculateDistance()
{
  long time;
  float distance;

  // Wait for impulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Get distance
  time = pulseIn(echoPin, HIGH);
  float time_float = time;
  distance = time_float / 58;

  return distance;
}

void moveStepper()
{
  if (positiveDirection)
  {
    if (currentPosition <= maxPosition - singleStep)
    {
      myStepper.step(singleStep);
      currentPosition = currentPosition + singleStep;
    }
    else
    {
      positiveDirection = false;
      delay(100);  // Delay before direction change
    }
  }
  else if (!positiveDirection)
  {
    if (currentPosition >= -maxPosition + singleStep)
    {
      myStepper.step(-singleStep);
      currentPosition = currentPosition - singleStep;
    }
    else
    {
      positiveDirection = true;
      delay(100);  // Delay before direction change
    }
  }
}

void readSerialInput()
{
  if (Serial.available() > 0)
  {
    inputData = Serial.readStringUntil('\n');

    if (inputData == "start" || inputData == "Start")
    {
      run = true;
    }
    else if (inputData == "stop" || inputData == "Stop")
    {
      run = false;
    }
    else if (inputData.startsWith("rotate") || inputData.startsWith("Rotate") && !run)
    {
      int angle = inputData.substring(6).toInt();
      myStepper.step(angle);
    }
    else if (inputData == "reset" || inputData == "Reset" && !run)
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
  currentPosition = 0;
  positiveDirection = true;
}

void readConfigButton()
{
  if (configMode && digitalRead(buttonPin) == LOW)
  {
    configMode = false;
    reset();
    delay(2000);
  }
  else if (!configMode && digitalRead(buttonPin) == LOW)
  {
    configMode = true;
    reset();
    delay(1000);
  }
}
