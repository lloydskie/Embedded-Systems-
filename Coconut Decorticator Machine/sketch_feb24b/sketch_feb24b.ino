#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET 4     // Reset pin # (or -1 if sharing Arduino reset pin)
#define Servo_PWM 6
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Servo MG995_Servo;

#define RT0 100000  // Ω
#define B 3977     // K
#define VCC 5      //Supply voltage
#define R 10000    //R=10KΩ

//Variables
float RT, VR, ln, TX, T0, VRT;
unsigned long previousMillis = 0;
const long interval = 10000; // 10 seconds interval

void setup()
{
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
  }
  T0 = 25 + 273.15;
  MG995_Servo.attach(Servo_PWM); // Attach servo to pin 9
  display.clearDisplay();
  display.drawPixel(10, 10, SSD1306_WHITE);
  display.display();
  delay(0);
}

void loop()
{
  unsigned long currentMillis = millis();

  VRT = analogRead(A0);               //Acquisition analog value of VRT
  VRT = (5.00 / 1023.00) * VRT;       //Conversion to voltage
  VR = VCC - VRT;
  RT = VRT / (VR / R);                //Resistance of RT
  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor
  TX = TX - 273.15;

  Serial.print("Temperature:");
  Serial.print("\t");
  Serial.print(TX);
  Serial.print("C\t\t");
  Serial.print(TX + 273.15);        //Conversion to Kelvin
  Serial.print("K\t\t");
  Serial.print((TX * 1.8) + 32);    //Conversion to Fahrenheit
  Serial.println("F");

  if (TX >= 35) // Check if temperature is 0 degrees Celsius or higher
  {
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;
      brakeApply(); // Apply the brake disc
    }
    else
    {
      brakeRelease(); // Release the brake disc
    }
  }

  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Temp = ");
  display.setCursor(40, 10);
  display.println(TX);
  display.setCursor(100, 10);
  display.println(F("C"));
  display.display();
  delay(0);
}

void brakeApply()
{
  // Apply brake by rotating servo clockwise to 90 degrees
  MG995_Servo.write(90);
}

void brakeRelease()
{
  // Release brake by rotating servo back to its default position
  MG995_Servo.write(0); // Change 0 to the default position angle if it's different
}
