#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display
unsigned long startTime;

void setup() {
  lcd.init();                      // initialize the lcd 
  lcd.backlight();                // Turn on the backlight
  lcd.setCursor(0, 0);
  lcd.print("RUN TIME");
  startTime = millis();          // Record the start time
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;

  // Calculate hours, minutes, and seconds
  unsigned long hours = elapsedTime / 3600000;
  elapsedTime %= 3600000;
  unsigned long minutes = elapsedTime / 60000;
  elapsedTime %= 60000;
  unsigned long seconds = elapsedTime / 1000;

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(hours);
  lcd.print(":");
  if (minutes < 10) lcd.print("0");
  lcd.print(minutes);
  lcd.print(":");
  if (seconds < 10) lcd.print("0");
  lcd.print(seconds);

  // If Arduino is reset, reset the timer
  if (digitalRead(3) == LOW) { // Assuming reset button is connected to pin 2
    startTime = millis();
  }
}
