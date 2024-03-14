#include <Servo.h>

Servo myServo;

const int button1Pin = 2; // Pin for the 1st button
const int button2Pin = 3; // Pin for the 2nd button

int angle1 = 70; // 1st angle (0 to 45)
int angle2 = 110; // 2nd angle (45 to 0)

void setup() {
  myServo.attach(9); // Connect the servo to pin 9
  pinMode(button1Pin, INPUT_PULLUP); // Set button1Pin as input with pull-up resistor
  pinMode(button2Pin, INPUT_PULLUP); // Set button2Pin as input with pull-up resistor
}

void loop() {
  if (digitalRead(button1Pin) == LOW) { // Check if button 1 is pressed
    moveServo(angle1); // Move servo to angle1
    delay(500); // Add a small delay for stability
  }

  if (digitalRead(button2Pin) == LOW) { // Check if button 2 is pressed
    moveServo(angle2); // Move servo to angle2
    delay(500); // Add a small delay for stability
  }
}

void moveServo(int angle) {
  myServo.write(angle); // Move the servo to the specified angle
  delay(1000); // Add a delay to allow the servo to reach the desired position
}
