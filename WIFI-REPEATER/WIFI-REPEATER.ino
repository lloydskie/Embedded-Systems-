#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Replace these with your WiFi network settings
const char* ssid = "UPHCollege"; // SSID of your primary WiFi
const char* password = "elearning223"; // Password of your primary WiFi

// This will be the SSID and password for the extended network
const char* extenderSSID = "Hello";
const char* extenderPassword = "World";

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Begin by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Now configure the AP side (extender)
  WiFi.softAP(extenderSSID, extenderPassword);

  Serial.println("Extender WiFi setup done.");
}

void loop() {
  // Check WiFi connection (optional)
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connection lost. Reconnecting...");
    WiFi.reconnect();
  }

  // Add your extender functionality here (optional)
  // For example, you could implement a simple web server 
  // to provide information about the extender or allow configuration changes.
}
