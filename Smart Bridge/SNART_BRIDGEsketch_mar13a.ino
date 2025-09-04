#include <Servo.h>

#define SOIL_MOISTURE_PIN A0
#define SERVO_PIN_1 9
#define SERVO_PIN_2 10

Servo servo1;
Servo servo2;

int moistureLevel;
int initialPosition = 0; // Initial position for servos (bridge up)

void setup() {
  Serial.begin(9600);
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  servo1.attach(SERVO_PIN_1);
  servo2.attach(SERVO_PIN_2);

  // Set initial position of servos to 90 degrees (bridge up)
  servo1.write(initialPosition);
  servo2.write(initialPosition);
  delay(1000);
}

void loop() {
  moistureLevel = analogRead(SOIL_MOISTURE_PIN);
  Serial.print("Soil Moisture Level: ");
  Serial.println(moistureLevel);

  if (moistureLevel > 500) { // Adjust threshold as needed
    moveBridgeDown();
  } else if (moistureLevel < 500 && (servo1.read() != initialPosition || servo2.read() != initialPosition)) {
    // Move bridge back to initial position if moisture level goes below threshold and servos are not already at initial position
    moveBridgeUp();
  }

  delay(500);
}

void moveBridgeDown() {
  // Move the bridge down by setting servo positions to 0 degrees
  servo1.write(90);  // Adjust angle for servo 1 if needed
  servo2.write(90);  // Adjust angle for servo 2 if needed
  delay(1000);       // Delay for bridge movement (adjust as needed)
}

void moveBridgeUp() {
  // Move the bridge back to initial position
  servo1.write(initialPosition);
  servo2.write(initialPosition);
  delay(1000);       // Delay for bridge movement (adjust as needed)
}
