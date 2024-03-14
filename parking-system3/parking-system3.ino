#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
String UID = "B3 D5 72 A5";

Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 rfid(SS_PIN, RST_PIN);

const int redLedPin = 5;
const int greenLedPin = 4;
const int buzzerPin = 2;

void setup() {
  Serial.begin(9600);
  servo.attach(6);  // Initialize the servo (Pin 6)
  servo.write(0);
  lcd.init();
  lcd.backlight();
  SPI.begin();
  rfid.PCD_Init();

  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  lcd.setCursor(4, 0);
  lcd.print("Mabuhay!");
  lcd.setCursor(1, 1);
  lcd.print("Put your card");

  if (!rfid.PICC_IsNewCardPresent())
    return;
  if (!rfid.PICC_ReadCardSerial())
    return;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scanning");
  Serial.print("NUID tag is: ");
  String ID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    lcd.print(".");
    ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    ID.concat(String(rfid.uid.uidByte[i], HEX));
    delay(300);
  }
  ID.toUpperCase();

  if (ID.substring(1) == UID) {
    servo.write(70);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Thank you!!!");
    digitalWrite(greenLedPin, HIGH);  // Turn on the green LED
    tone(buzzerPin, 1000);  // Sound the buzzer
    delay(1500);
    noTone(buzzerPin);  // Stop the buzzer sound
    digitalWrite(greenLedPin, LOW);  // Turn off the green LED
    lcd.clear();
    servo.write(0); // Close the gate
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Insufficient Bal");
    digitalWrite(redLedPin, HIGH);  // Turn on the red LED
    tone(buzzerPin, 2000);  // Sound the buzzer
    delay(1500);
    noTone(buzzerPin);  // Stop the buzzer sound
    digitalWrite(redLedPin, LOW);  // Turn off the red LED
    lcd.clear();
  }
}
