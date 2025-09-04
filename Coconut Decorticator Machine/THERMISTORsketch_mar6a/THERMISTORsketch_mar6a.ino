#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display

const int thermistorPin = A1; // Analog pin for reading the thermistor
const int thermistorNominal = 10000; // Resistance at 25 degrees Celsius
const int temperatureNominal = 25; // Temperature for nominal resistance
const int BCoefficient = 3950; // Beta coefficient of the thermistor

void setup() {
  lcd.init();                      // Initialize the LCD
  lcd.backlight();                 // Turn on the backlight

  // Print a message on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");

  Serial.begin(9600); // Start serial communication
}

void loop() {
  int rawADC = analogRead(thermistorPin); // Read the ADC value from the thermistor
  float resistance = 10000 / ((1023.0 / rawADC) - 1); // Calculate the resistance of the thermistor
  float temperature = 1 / (log(resistance / 10000) / BCoefficient + 1 / (temperatureNominal + 273.15)) - 273.15; // Calculate the temperature in Celsius

  // Print temperature on the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  // Print temperature on the LCD
  lcd.setCursor(0, 1);
  lcd.print("     "); // Clear the previous temperature value
  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.print(" C");

  delay(100); // Delay before taking the next temperature reading
}
