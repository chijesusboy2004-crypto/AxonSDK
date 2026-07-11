#include "AxonESPNow.h"

#ifdef AXON_USE_ESPNOW

bool AxonESPNow::_initialized = false;
AxonESPNowCallback AxonESPNow::_onReceiveCallback = nullptr;

void AxonESPNow::onDataRecv(const uint8_t* mac_addr, const uint8_t* data, int data_len) {
    char buffer[250];
    int len = data_len < 249 ? data_len : 249;
    memcpy(buffer, data, len);
    buffer[len] = '\0';

    char* colon = strchr(buffer, ':');
    if (colon) {
        *colon = '\0';
        const char* vpin = buffer;
        const char* value = colon + 1;

        if (_onReceiveCallback) {
            _onReceiveCallback(vpin, value);
        } else {
            // Default: write to the global VPIN manager
            Axon.virtualWrite(vpin, value);
        }
    }
}

bool AxonESPNow::begin(AxonESPNowCallback callback) {
    if (_initialized) return true;
    
    _onReceiveCallback = callback;
    WiFi.mode(WIFI_AP_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("[AXON ESP-NOW] Initialization failed!");
        return false;
    }

    // Register callback for receiving data
#if defined(ESP32)
    esp_now_register_recv_cb(onDataRecv);
#elif defined(ESP8266)
    esp_now_register_recv_cb(reinterpret_cast<esp_now_recv_cb_t>(onDataRecv));
#endif

    _initialized = true;
    Serial.println("[AXON ESP-NOW] Initialized successfully.");
    return true;
}

bool AxonESPNow::addPeer(const uint8_t* mac) {
    if (esp_now_is_peer_exist(mac)) return true;

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, mac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("[AXON ESP-NOW] Failed to add peer!");
        return false;
    }
    return true;
}

bool AxonESPNow::send(const uint8_t* mac, const char* vpin, const char* value) {
    char payload[128];
    snprintf(payload, sizeof(payload), "%s:%s", vpin, value);
    
    esp_err_t result = esp_now_send(mac, (uint8_t*)payload, strlen(payload));
    return (result == ESP_OK);
}

#endif
