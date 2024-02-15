
#include <DHTesp.h>
#include <WiFi.h>
#include <ThingsBoard.h>

#define DHTTYPE    DHT22 // DHT11, DHT22, DHT21
#define DHT_PIN    4
#define TOKEN     "21asd7"
String teamNumber = "A";
#define THINGSBOARD_SERVER  "10.0.1.2" 
#define SERIAL_BAUD 115200 // 115200 is the NorseIoT club standard
WiFiClient espClient;
ThingsBoard tb(espClient);
int status = WL_IDLE_STATUS;
DHTesp dht;
int quant = 20;
int send_delay = 2000;
int send_passed = 0;

// Separate each line with a \n. 
String inputMessage = "and the\nINTERalliance\nwelcome Team " + teamNumber + " to TechOlympics 2024! \n";


void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
  InitWiFi();
  dht.setup(DHT_PIN, DHTesp::DHTTYPE);




  // Print intro message.
  int maxArtLength = 0;
  String asciiArt[] = {
    "    _   __                     ____    ______",
    "   / | / /___  _____________  /  _/___/_  __/",
    "  /  |/ / __ \\/ ___/ ___/ _ \\ / // __ \\/ /   ",
    " / /|  / /_/ / /  (__  )  __// // /_/ / /    ",
    "/_/ |_/\\____/_/  /____/\\___/___/\\____/_/     "
  };

  for (int i = 0; i < sizeof(asciiArt) / sizeof(asciiArt[0]); i++) {
    int lineLength = asciiArt[i].length();
    if (lineLength > maxArtLength) {
      maxArtLength = lineLength;
    }
  }

  int totalLines = 0;
  int maxMessageLength = 0;
  int startIndex = 0;

  for (int i = 0; i < inputMessage.length(); i++) {
    if (inputMessage.charAt(i) == '\n' || i == inputMessage.length() - 1) {
      totalLines++;
      String line = inputMessage.substring(startIndex, i);
      int currentLength = line.length();
      if (currentLength > maxMessageLength) {
        maxMessageLength = currentLength;
      }
      startIndex = i + 1;
    }
  }

  int totalWidth = max(maxArtLength, maxMessageLength);
  for (int i = 0; i < sizeof(asciiArt) / sizeof(asciiArt[0]); i++) {
    centerAndPrintLine(asciiArt[i], totalWidth);
  }
  startIndex = 0;
  for (int i = 0; i < totalLines; i++) {
    int endIndex = inputMessage.indexOf('\n', startIndex);
    String line = inputMessage.substring(startIndex, endIndex); // Extract each line
    centerAndPrintLine(line, totalWidth);
    startIndex = endIndex + 1;
  }
  Serial.print("+");
  for (int j = 0; j < totalWidth + 4; j++) { // Add 4 extra spaces
    Serial.print("-");
  }
  Serial.println("+");
}



void loop() {
}