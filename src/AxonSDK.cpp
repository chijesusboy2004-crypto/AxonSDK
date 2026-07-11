#include "AxonSDK.h"

AxonSDK Axon; // Global instance

void axonMqttCallback(char* topic, uint8_t* payload, unsigned int length) {
    char* lastSlash = strrchr(topic, '/');
    if (lastSlash) {
        const char* vpin = lastSlash + 1;
        char valueStr[64];
        unsigned int len = length < 63 ? length : 63;
        memcpy(valueStr, payload, len);
        valueStr[len] = '\0';
        
        Axon.handleWrite(vpin, valueStr);
    }
}

AxonSDK::AxonSDK() : _deviceToken(nullptr), _deviceSecret(nullptr), _writeHandlerCount(0) {
    // Constructor
}

AxonSDK::~AxonSDK() {
    // Destructor
}

void AxonSDK::begin(const char* deviceToken, const char* deviceSecret, Client* customClient) {
    _deviceToken = deviceToken;
    _deviceSecret = deviceSecret;
    
    if (customClient) {
        _mqttClient.setClient(customClient);
    }
    _mqttClient.begin(_deviceToken, _deviceSecret);
    _mqttClient.setCallback(axonMqttCallback);
    
    AxonLogger::info("SDK Initialized.");
}

void AxonSDK::setWriteHandler(const char* vpin, AxonWriteHandler handler) {
    if (_writeHandlerCount < AXON_MAX_PIN_HANDLERS) {
        _writeHandlers[_writeHandlerCount++] = { vpin, handler };
    }
}

void AxonSDK::handleWrite(const char* vpin, const char* value) {
    for (int i = 0; i < _writeHandlerCount; i++) {
        if (strcmp(_writeHandlers[i].vpin, vpin) == 0) {
            _writeHandlers[i].handler(value);
            return;
        }
    }
}

bool AxonSDK::connectWiFi(const char* ssid, const char* password) {
    return _wifiManager.connect(ssid, password);
}

bool AxonSDK::connect() {
    if (!_wifiManager.isConnected()) {
        AxonLogger::error("Must connect to WiFi first.");
        return false;
    }
    return _mqttClient.connect();
}

void AxonSDK::virtualWrite(const char* vpin, float value) {
    _vpinManager.write(vpin, value);
}

void AxonSDK::virtualWrite(const char* vpin, const char* value) {
    _vpinManager.write(vpin, value);
}

void AxonSDK::virtualWrite(const char* vpin, int value) {
    _vpinManager.write(vpin, value);
}

float AxonSDK::virtualReadFloat(const char* vpin) {
    return _vpinManager.readFloat(vpin);
}

int AxonSDK::virtualReadInt(const char* vpin) {
    return _vpinManager.readInt(vpin);
}

const char* AxonSDK::virtualReadString(const char* vpin) {
    return _vpinManager.readString(vpin);
}

void AxonSDK::run() {
    _wifiManager.maintain();
    _mqttClient.maintain();
}

void AxonSDK::publishTelemetry() {
    if (_vpinManager.hasUpdates() && _mqttClient.isConnected()) {
        char payload[1024];
        if (_vpinManager.buildUpdateJson(payload, sizeof(payload))) {
            char topic[128];
            snprintf(topic, sizeof(topic), "devices/%s/telemetry", _deviceToken);
            _mqttClient.publish(topic, payload);
            _vpinManager.clearUpdates();
            AxonLogger::info("Published telemetry batch.");
        }
    }
}

void AxonSDK::log(const char* message) {
    AxonLogger::log("USER", message);
}
