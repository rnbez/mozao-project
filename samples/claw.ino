#include <Servo.h>

#define SENSOR_PIN 31
#define SERVO_PIN 33
#define SERVO_OPEN_POS 90

Servo srv;
int detectionRead = 0;
int pos = 0;

void setup()
{
    srv.attach(SERVO_PIN);
    srv.write(SERVO_OPEN_POS);
    pinMode(SENSOR_PIN, INPUT);
    Serial.begin(9600);
}

void loop()
{
    detectionRead = digitalRead(SENSOR_PIN);
    if (detectionRead == 0)
    {
        Serial.println("Objeto : Detectado");
        for (pos = SERVO_OPEN_POS; pos >= 0; pos -= 1)
        {
            srv.write(pos);
            delay(5);
        }
        for (pos = 0; pos <= SERVO_OPEN_POS; pos += 1)
        {
            srv.write(pos);
            delay(10);
        }
        srv.write(SERVO_OPEN_POS);
    }
}
