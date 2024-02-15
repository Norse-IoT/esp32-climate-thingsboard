/*

This file is used to assembly-line image all of the ESP32s used for TechOlympics.

Change the letter indicated in the include statement to easily cycle between code.

*/

/*
Temp and humidity sensor connected to ThingsBoard

ArduinoJSON by Benoit Blanchon: 6.20.1
ArduinoHttpClient by Arduino: 0.5.0
DHT sensor library by begee_tokyo: 1.1.9
PubSubClient by Nick O'Leary: 2.8
ThingsBoard by ThingsBoard Team: 0.2.0
*/

// declarations
void InitWiFi();
void blinkLED(int, int, int);
void centerAndPrintLine(String, int);


#define WIFI_AP_NAME        ""
#define WIFI_PASSWORD       ""

// NOTE: change the letter on this:
#include "board_code/starter_code_a.cpp"

// common definitions
void blinkLED(int numberOfFlashes, int durationOn, int durationOff) {
  for (int i = 0; i < numberOfFlashes; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(durationOn);
    digitalWrite(LED_BUILTIN, LOW);
    delay(durationOff);
  }
}

void InitWiFi() {
  Serial.println("");
  Serial.print("Connecting to AP as ");
  Serial.print(WiFi.macAddress());

  // Attempt to connect to WiFi network
  WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  blinkLED(3, 120, 120);

  Serial.println("");
  Serial.print("Connected to AP with IP address ");
  Serial.println(WiFi.localIP());
}

void centerAndPrintLine(String line, int maxLength) {
  int lineLength = line.length();
  int padding = (maxLength - lineLength) / 2;

  // Center the line and print it
  for (int j = 0; j < padding; j++) {
    Serial.print(" ");
  }
  Serial.print("  "); // Two extra spaces on the left
  Serial.print(line);
  Serial.print("  "); // Two extra spaces on the right
  Serial.println();
}