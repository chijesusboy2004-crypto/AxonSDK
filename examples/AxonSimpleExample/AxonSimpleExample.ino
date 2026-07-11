/*
 * AXON IoT Platform - Simple ESP32 DHT11 & LED Controller Example
 * 
 * Demonstrates the Blynk-style macro configuration model.
 * Automatically installs libraries from the Arduino Library Manager.
 */

// 1. Configure AXON credentials
#define AXON_TEMPLATE_ID "axon-hub-10204"
#define AXON_DEVICE_NAME "MyDevice"
#define AXON_AUTH_TOKEN  "axon_sec_default"

// 2. Configure WiFi Credentials
#define WIFI_SSID       "YOUR_WIFI_SSID"
#define WIFI_PASSWORD   "YOUR_WIFI_PASSWORD"

// 3. Include the AXON Library
#include <axon.h>

// Pins mapping
#define DHTPIN 32        // DHT11 sensor pin
#define LED_PIN 23       // LED controller pin

unsigned long lastSend = 0;

// 4. Register write action callback (Blynk-style)
// Triggers when V4 state changes on the AXON dashboard
AXON_WRITE(V4) {
    int value = param.asInt();
    digitalWrite(LED_PIN, value == 1 ? HIGH : LOW);
    Serial.printf("[LED Action] LED pin V4 set to %s\n", value == 1 ? "HIGH" : "LOW");
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW); // Default off
    
    // 5. Connect and register
    Axon.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("AXON Setup complete. Stream active.");
}

void loop() {
    // Maintains connection and processes callbacks
    Axon.run();

    unsigned long now = millis();
    // Publish sensor data every 10 seconds
    if (now - lastSend >= 10000) {
        lastSend = now;
        
        // Mocking DHT11 read (replace with actual dht.readTemperature() / dht.readHumidity() in production)
        float temperature = 24.5f + (random(-10, 10) / 10.0f);
        float humidity = 62.0f + (random(-20, 20) / 10.0f);
        
        // Write to Virtual Pins V1 and V2
        Axon.virtualWrite("V1", temperature);
        Axon.virtualWrite("V2", humidity);
        
        Serial.printf("[Sensor Data] Temperature: %.2f C, Humidity: %.2f %%\n", temperature, humidity);
    }
}
