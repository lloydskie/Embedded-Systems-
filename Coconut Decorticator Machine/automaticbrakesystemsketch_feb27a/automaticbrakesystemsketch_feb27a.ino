#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal_I2C.h>
#include <CapacitiveSensor.h>
#include <Servo.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display

#define RT0 100000   // Ω
#define B 3977      // K
#define VCC 5    //Supply voltage
#define R 10000  //R=10KΩ

#define RELAY_PIN 7 // Pin connected to the relay module

//Variables
float RT, VR, ln, TX, T0, VRT;

CapacitiveSensor capSensor = CapacitiveSensor(4, 2);
Servo servoMotor1;
Servo servoMotor2;
int threshold = 2000;
const int ledPin = 13;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  servoMotor1.attach(9); // Attach the servo motor 1 to pin 9
  servoMotor2.attach(10); // Attach the servo motor 2 to pin 10
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Initially, keep the relay off
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  T0 = 25 + 273.15;  
  display.clearDisplay();
  display.drawPixel(10, 10, SSD1306_WHITE);
  display.display();
  delay(2000);
}

void loop() {

  VRT = analogRead(A0);              //Acquisition analog value of VRT
  VRT = (5.00 / 1023.00) * VRT;      //Conversion to voltage
  VR = VCC - VRT;
  RT = VRT / (VR / R);               //Resistance of RT
  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor
  TX = TX - 273.15;  

  long sensorValue = capSensor.capacitiveSensor(10);

  Serial.print("Temperature:");
  Serial.print("\t");
  Serial.print(TX);
  Serial.print("C\t\t");
  Serial.print(TX + 273.15);        //Conversion to Kelvin
  Serial.print("K\t\t");
  Serial.print((TX * 1.8) + 32);    //Conversion to Fahrenheit
  Serial.println("F");

  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Temp = ");
  display.setCursor(40, 10);
  display.setTextSize(2); 
  display.println(TX);
  display.setCursor(100, 10);
  display.setTextSize(1);
  display.println(F("C"));
  display.display();

  // // Activate the solenoid if temperature rises to 40 degrees Celsius
  // if (TX >= 40) {
  //   digitalWrite(RELAY_PIN, HIGH); // Turn on the relay
  // } else {
  //   digitalWrite(RELAY_PIN, LOW); // Turn off the relay
  // }

  // if (sensorValue > threshold) {
  //   digitalWrite(ledPin, HIGH);
  //   servoMotor.write(0); // Rotate servo to 0 degrees
  //   delay(1000); // Adjust delay according to your servo's speed and desired rotation
  //   servoMotor.write(90); // Rotate servo to 90 degrees
  //   delay(1000); // Adjust delay according to your servo's speed and desired rotation
  //   digitalWrite(ledPin, LOW);
  // }

  if (sensorValue >= threshold) {
    
    digitalWrite(ledPin, HIGH); // Turn on the LED
  } else {
    
    digitalWrite(ledPin, LOW); // Turn off the LED
  }

  if (sensorValue > threshold) {
    servoMotor2.write(70);
    delay(1000);
  }

  if (sensorValue > threshold) {
    // digitalWrite(ledPin, HIGH);
    servoMotor1.write(0); // Rotate servo to 0 degrees
    delay(500); // Adjust delay according to your servo's speed and desired rotation
    servoMotor1.write(90); // Rotate servo to 90 degrees
    delay(500); // Adjust delay according to your servo's speed and desired rotation
    // digitalWrite(ledPin, LOW);
  }


  delay(10);
}
