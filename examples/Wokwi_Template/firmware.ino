// AXON IoT - Wokwi Simulation Template
#include <Arduino.h>
#include "AxonSDK.h"
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define RELAY_PIN 2

// Copy your Device Token and Secret from the AXON Dashboard
const char* DEVICE_TOKEN = "paste_your_device_token_here";
const char* DEVICE_SECRET = "paste_your_device_secret_here";

// Wokwi uses a virtual WiFi gateway
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASS = "";

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);

  // Initialize AXON SDK
  Axon.begin(DEVICE_TOKEN, DEVICE_SECRET);
  Axon.connectWiFi(WIFI_SSID, WIFI_PASS);
  Axon.connect();
}

void loop() {
  Axon.run();

  // Every 5 seconds, read sensor and push telemetry
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 5000) {
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (!isnan(temp)) {
      Axon.virtualWrite("V1", temp); // V1 = Temperature
      Axon.virtualWrite("V2", hum);  // V2 = Humidity
      Axon.publishTelemetry();
    }
    lastUpdate = millis();
  }
}
