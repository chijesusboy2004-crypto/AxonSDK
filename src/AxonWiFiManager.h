#ifndef AXON_WIFI_MANAGER_H
#define AXON_WIFI_MANAGER_H

#include <Arduino.h>

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

class AxonWiFiManager {
public:
    AxonWiFiManager();
    ~AxonWiFiManager();

    bool connect(const char* ssid, const char* password);
    bool isConnected();
    void maintain(); // Reconnect if disconnected

private:
    const char* _ssid;
    const char* _password;
    unsigned long _lastAttempt;
};

#endif
