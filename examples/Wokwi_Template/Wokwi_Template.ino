// AXON IoT - Wokwi Simulation Template
#define AXON_TEMPLATE_ID "your_template_id"
#define AXON_DEVICE_NAME "WokwiDevice"
#define AXON_AUTH_TOKEN  "paste_your_device_token_here"

#include <axon.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define RELAY_PIN 2

// Actuator listener
AXON_WRITE(V4) {
    int value = param.asInt();
    digitalWrite(RELAY_PIN, value == 1 ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);

  // Initialize AXON SDK (Wokwi uses virtual WiFi gateway Wokwi-GUEST)
  Axon.begin("Wokwi-GUEST", "");
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
    }
    lastUpdate = millis();
  }
}
