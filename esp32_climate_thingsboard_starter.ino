/*
Temp and humidity sensor connected to ThingsBoard
*/

 // Baud rate for serial output
#define SERIAL_BAUD    // how fast should our computer talk to the microcontroller? Needs an integer like 115200

// Include the necessary code libraries. They are DHTesp.h,  WiFi.h, and ThingsBoard.h. The format is:
// #include <TheNameOfTheLibrary>



// Which model of humidity/temp sensor?
#define DHTTYPE    // DHT11, DHT22, DHT21

// ESP32 pin used to query the sensor
#define DHT_PIN // This needs an integer for the specific pin

// See https://thingsboard.io/docs/getting-started-guides/helloworld/ 
// to understand how to obtain an access token
#define TOKEN     // This needs a string from the list below that matches the number of your device
// 000: x98xgy 
// 001: 21asd7
// 002: lkaskk
// 003: 2sgg23
// 004: 24lehs
// 005: 890wkd
// 006: 1rodx2
// 007: q3qh1n
// 008: daex3l
// 009: 2bwa4z
// 010: wpc89e
// 011: en4mea
// 012: sms2pt
// 013: xvw0bc
// 014: owooj1
// 015: 92j3vp
// 016: 0iJh56

// ThingsBoard server. 
#define THINGSBOARD_SERVER   //This is a string with an IP address value of 10.0.1.2

// WiFi access point
#define WIFI_AP_NAME        //This is a string with the value NORSEIOT2G
// WiFi password
#define WIFI_PASSWORD       //This is a string with the value freezerburn15

// How often should we send temperature/humidity data (in milliseconds)?
int send_delay =  ;//need an integer here!








// Initialize ThingsBoard client
WiFiClient espClient;

// Instantiate a ThingsBoard object
ThingsBoard tb(espClient);

// Get WiFi radio status
int status = WL_IDLE_STATUS;

// Instantiate a DHT object
DHTesp dht;

// Main application loop delay
int quant = 20;

// Time passed after temperature/humidity data was sent in milliseconds.
int send_passed = 0;

// Set up the application
void setup() {
  // Initialize serial for debugging
  Serial.begin(SERIAL_BAUD);

  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
  
  InitWiFi();

  // Initialize temperature sensor
  dht.setup(DHT_PIN, DHTesp::DHTTYPE);
}


// Main application loop
void loop() {
  delay(quant);

  send_passed += quant;

  // Reconnect to WiFi if needed
  if (WiFi.status() != WL_CONNECTED) {
    InitWiFi();
    return;
  }

  // Reconnect to ThingsBoard, if needed
  if (!tb.connected()) {
    // Connect to the ThingsBoard
    Serial.print("Connecting to ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed!");
      return;
    }
  }

  // Check if it is time to send telemetry
  if (send_passed > send_delay) {
    Serial.print("Sending data ");

    // Uploads new telemetry to ThingsBoard using MQTT.
    // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api

    // Create a structure named "measurements" to hold humidity and temperature values
    TempAndHumidity measurements = dht.getTempAndHumidity();    

    if (isnan(measurements.humidity) || isnan(measurements.temperature)) {
      Serial.println("**Failed to read DHT sensor!**");
    } else {
      tb.sendTelemetryFloat("temperature", measurements.temperature);
      tb.sendTelemetryFloat("humidity", measurements.humidity);
      Serial.print(measurements.temperature);
      Serial.print("c / ");
      Serial.print(measurements.humidity);
      Serial.println("%).");
      blinkLED(1,250,0);
    }

    send_passed = 0;
  }

  // Process messages
  tb.loop();
}

void InitWiFi()
{
  Serial.println("");
  Serial.print("Connecting to AP as ");
  Serial.print(WiFi.macAddress());
  
  // Attempt to connect to WiFi network
  WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  blinkLED(3,120,120);

  Serial.println("");
  Serial.print("Connected to AP with IP address ");
  Serial.println(WiFi.localIP());
}

void blinkLED(int numberOfFlashes, int durationOn, int durationOff) {
  for(int i = 0; i < numberOfFlashes; i++){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(durationOn);
    digitalWrite(LED_BUILTIN, LOW);
    delay(durationOff);
  }
}
