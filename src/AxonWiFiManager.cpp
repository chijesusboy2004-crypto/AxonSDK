#include "AxonWiFiManager.h"

AxonWiFiManager::AxonWiFiManager() : _ssid(nullptr), _password(nullptr), _lastAttempt(0) {}

AxonWiFiManager::~AxonWiFiManager() {}

bool AxonWiFiManager::connect(const char* ssid, const char* password) {
    _ssid = ssid;
    _password = password;

    Serial.print("[AXON WiFi] Connecting to ");
    Serial.println(_ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("[AXON WiFi] Connected! IP: ");
        Serial.println(WiFi.localIP());
        return true;
    } else {
        Serial.println("[AXON WiFi] Connection failed.");
        return false;
    }
}

bool AxonWiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void AxonWiFiManager::maintain() {
    if (!isConnected()) {
        if (millis() - _lastAttempt > 10000) { // Try to reconnect every 10s
            Serial.println("[AXON WiFi] Disconnected! Attempting to reconnect...");
            WiFi.disconnect();
            WiFi.begin(_ssid, _password);
            _lastAttempt = millis();
        }
    }
}
