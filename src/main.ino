/*
 * AUTHOR: Rafael Bezerra (https://github.com/rnbez)
 * BASED ON:
 *  - ./zumo-examples/examples/LineFollower/LineFollower.ino
 */

#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>

#define MAX_SPEED 400
#define CALIB_LED_PIN 13

ZumoBuzzer buzzer;
ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
int lastError = 0;

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

void setup()
{
  buzzer.play(">g32>>c32"); // Play welcome song

  reflectanceSensors.init(); // Initialize the reflectance sensors module

  button.waitForButton(); // Wait for button click

  pinMode(CALIB_LED_PIN, OUTPUT);
  digitalWrite(CALIB_LED_PIN, HIGH); // calibration LED on
  calibrate();
  motors.setSpeeds(0, 0);
  digitalWrite(CALIB_LED_PIN, LOW); // calibration LED off

  buzzer.play(">g32>>c32");

  button.waitForButton(); // Wait for button click
}

void loop()
{
  unsigned int sensors[6];
  int position = reflectanceSensors.readLine(sensors);

  int error = position - 2500; // 2500 corresponds to position to the center
  int speedDifference = error / 4 + 6 * (error - lastError);
  lastError = error;

  int m1Speed = constrain(MAX_SPEED + speedDifference, 0, MAX_SPEED);
  int m2Speed = constrain(MAX_SPEED - speedDifference, 0, MAX_SPEED);

  motors.setSpeeds(m1Speed, m2Speed);
}
