#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
String UID = "B3 D5 72 A5";

Servo servo;
MFRC522 rfid(SS_PIN, RST_PIN);

const int redLedPin = 5;
const int greenLedPin = 4;
const int buzzerPin = 2;

void setup() {
  Serial.begin(9600);
  servo.attach(6);  // Initialize the servo (Pin 6)
  servo.write(0);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent())
    return;
  if (!rfid.PICC_ReadCardSerial())
    return;

  Serial.print("NUID tag is: ");
  String ID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    ID.concat(String(rfid.uid.uidByte[i], HEX));
    delay(300);
  }
  ID.toUpperCase();

  if (ID.substring(1) == UID) {
    servo.write(70);
    digitalWrite(greenLedPin, HIGH);  // Turn on the green LED
    tone(buzzerPin, 1000);  // Sound the buzzer
    delay(1500);
    noTone(buzzerPin);  // Stop the buzzer sound
    digitalWrite(greenLedPin, LOW);  // Turn off the green LED
    servo.write(0); // Close the gate
  } else {
    digitalWrite(redLedPin, HIGH);  // Turn on the red LED
    tone(buzzerPin, 2000);  // Sound the buzzer
    delay(1500);
    noTone(buzzerPin);  // Stop the buzzer sound
    digitalWrite(redLedPin, LOW);  // Turn off the red LED
  }
}
