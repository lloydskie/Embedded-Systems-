#include <CapacitiveSensor.h>
#include <Servo.h>

CapacitiveSensor capSensor = CapacitiveSensor(4, 2);
Servo servoMotor;
int threshold = 2000;
const int ledPin = 12;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  servoMotor.attach(9); // Attach the servo motor to pin 9
}

void loop() {
  long sensorValue = capSensor.capacitiveSensor(30);
  Serial.println(sensorValue);

  if (sensorValue > threshold) {
    digitalWrite(ledPin, HIGH);
    servoMotor.write(0); // Rotate servo to 0 degrees
    delay(1000); // Adjust delay according to your servo's speed and desired rotation
    servoMotor.write(90); // Rotate servo to 90 degrees
    delay(1000); // Adjust delay according to your servo's speed and desired rotation
    digitalWrite(ledPin, LOW);
  }

  delay(10);
}
