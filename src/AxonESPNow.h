#ifndef AXON_ESPNOW_H
#define AXON_ESPNOW_H

#include <Arduino.h>
#include "AxonSDK.h"

// ESP-NOW is supported on ESP32/ESP8266 boards.
// User must define AXON_USE_ESPNOW.
#ifdef AXON_USE_ESPNOW
#include <esp_now.h>
#include <WiFi.h>

typedef void (*AxonESPNowCallback)(const char* vpin, const char* value);

class AxonESPNow {
private:
    static AxonESPNowCallback _onReceiveCallback;
    static bool _initialized;

    static void onDataRecv(const uint8_t* mac_addr, const uint8_t* data, int data_len);

public:
    static bool begin(AxonESPNowCallback callback = nullptr);
    static bool addPeer(const uint8_t* mac);
    static bool send(const uint8_t* mac, const char* vpin, const char* value);
};
#endif

#endif // AXON_ESPNOW_H
