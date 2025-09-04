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

  // Activate the solenoid if temperature rises to 40 degrees Celsius
  // if (TX >= 40) {
  //   digitalWrite(RELAY_PIN, HIGH); // Turn on the relay
  // } else {
  //   digitalWrite(RELAY_PIN, LOW); // Turn off the relay
  // }

  if (sensorValue >= threshold) {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(ledPin, HIGH); // Turn on the LED
  } else {
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(ledPin, LOW); // Turn off the LED
  }

  delay(10);
}
