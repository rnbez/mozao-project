/*
 * AUTHOR: Rafael Bezerra (https://github.com/rnbez)
 * BASED ON:
 *  - ./zumo-examples/examples/LineFollower/LineFollower.ino
 */

#include <NewPing.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>

#define MIN_SPEED 0
#define MAX_SPEED 400
#define CALIB_LED_PIN 13
#define TRIGGER_PIN 49   
#define ECHO_PIN 47      
#define MAX_DISTANCE 200

ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
int lastError = 0;
int count = 50;

void calibrate()
{
  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  int i;
  for (i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
      motors.setSpeeds(-200, 200);
    else
      motors.setSpeeds(200, -200);
    reflectanceSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(20); 
  }
}

void turnLeft() {
  int i;
  for (i = 0; i < 22; i++)
  {
    motors.setSpeeds(-200, 200);
    delay(20); 
  }
  motors.setSpeeds(0, 0);
}

void turnRight() {
  int i;
  for (i = 0; i < 22; i++)
  {
    motors.setSpeeds(200, -200);
    delay(20); 
  }
  motors.setSpeeds(0, 0);
}

void goAhead() {
  int i;
  for (i = 0; i < 23; i++)
  {
    motors.setSpeeds(200, 200);
    delay(20); 
  }
  motors.setSpeeds(0, 0);
}

void dodge() {
  motors.setSpeeds(0, 0);
  turnLeft();
  goAhead();
  turnRight();
  goAhead();
  goAhead();
  turnRight();
  goAhead();
  turnLeft();
  motors.setSpeeds(0, 0);
}

void setup()
{
  // Serial.begin(115200);
  Serial.begin(9600);

  Serial.println("\n\n\nStarting...");

  reflectanceSensors.init(); // Initialize the reflectance sensors module

  button.waitForButton(); // Wait for button click

  Serial.println("Calibrating...");
  pinMode(CALIB_LED_PIN, OUTPUT);
  digitalWrite(CALIB_LED_PIN, HIGH); // calibration LED on
  calibrate();
  motors.setSpeeds(0, 0);
  digitalWrite(CALIB_LED_PIN, LOW); // calibration LED off

  button.waitForButton(); // Wait for button click
}

void loop()
{
  if (count == 0) {
    int dist = sonar.ping_cm();
    if (dist <= 5)
    {
      Serial.print("Ping: ");
      Serial.print(dist); // Send ping, get distance in cm and print result (0 = outside set distance range)
      Serial.println(" cm");
      dodge();
      count = 50;
      return;
    }
  }
  count--;

  unsigned int sensors[6];
  int pos = reflectanceSensors.readLine(sensors);

  int error = pos - 2500; // 2500 corresponds to position to the center
  int speedDifference = error / 4 + 6 * (error - lastError);
  lastError = error;

  int m1Speed = constrain(MAX_SPEED + speedDifference, MIN_SPEED, MAX_SPEED);
  int m2Speed = constrain(MAX_SPEED - speedDifference, MIN_SPEED, MAX_SPEED);

  // detect when zumo is lifted from the ground
  // if (pos == 0 || pos == 2500)
  // {
  //   m1Speed = MIN_SPEED;
  //   m2Speed = MIN_SPEED;
  // }

  motors.setSpeeds(m1Speed, m2Speed);
}
