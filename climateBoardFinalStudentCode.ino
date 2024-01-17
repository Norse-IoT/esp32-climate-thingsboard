#define SERIAL_BAUD    115200

#include <DHTesp.h>
#include <WiFi.h>
#include <ThingsBoard.h>



#define DHTTYPE    DHT22 // DHT11, DHT22, DHT21
#define DHT_PIN    4
#define TOKEN     "21asd7"
#define THINGSBOARD_SERVER  "10.0.1.2" 
#define WIFI_AP_NAME        ""
#define WIFI_PASSWORD       ""
WiFiClient espClient;
ThingsBoard tb(espClient);
int status = WL_IDLE_STATUS;
DHTesp dht;
int quant = 20;
int send_delay = 2000;
int send_passed = 0;


void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
  
  InitWiFi();

  dht.setup(DHT_PIN, DHTesp::DHTTYPE);
}



void loop() {
  delay(quant);

  send_passed += quant;

  if (WiFi.status() != WL_CONNECTED) {
    InitWiFi();
    return;
  }

  if (!tb.connected()) {
    Serial.print("Connecting to ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed!");
      return;
    }
  }

  if (send_passed > send_delay) {
    Serial.print("Sending data ");


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

  tb.loop();
}

void InitWiFi()
{
  Serial.println("");
  Serial.print("Connecting to AP as ");
  Serial.print(WiFi.macAddress());
  
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
