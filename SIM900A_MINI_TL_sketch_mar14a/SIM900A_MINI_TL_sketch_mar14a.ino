#include <SoftwareSerial.h>

SoftwareSerial myGSM(8,9);
int butswitch = 7;
int butstate;
int count = 0;

void setup() {
  pinMode(butswitch, INPUT_PULLUP);
  myGSM.begin(19200); //Setting the baud rate of GSM Module
  delay(100);
}

void loop() {
  butstate = digitalRead(butswitch);

  if (butstate == LOW) {
    SendMessage();
  }
}

void SendMessage() {
  myGSM.println("AT+CMGF=1"); //AT command that Sets the GSM Module in Text Mode
  delay(100); // 100 milliseconds delay
  myGSM.println("AT+CMGS=\"09937874632\"\r"); //AT command that assigns the mobile number of the SMS recipient
  delay(100);
  myGSM.println("ARDUINOTRONICS gsm test..."); // The SMS you want to send
  delay(100);
  myGSM.println((char)26); // ASCII code of CTRL+Z
  delay(100);
}
