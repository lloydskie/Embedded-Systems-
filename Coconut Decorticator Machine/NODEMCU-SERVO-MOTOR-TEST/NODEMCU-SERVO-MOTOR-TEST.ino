#include <Servo.h>

Servo myservo;  // Create servo object to control a servo

int pos = 0;    // Variable to store the servo position

void setup() {
  myservo.attach(D4);  // Attach the servo to pin D4
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) {  // Go from 0 degrees to 180 degrees
    myservo.write(pos);              // Set the servo position
    delay(10);                       // Wait 15 milliseconds for the servo to move
  }
  for (pos = 180; pos >= 0; pos -= 1) {  // Go from 180 degrees to 0 degrees
    myservo.write(pos);              // Set the servo position
    delay(10);                       // Wait 15 milliseconds for the servo to move
  }
}
