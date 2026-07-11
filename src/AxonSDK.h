#ifndef AXON_SDK_H
#define AXON_SDK_H

#include <Arduino.h>
#include "AxonWiFiManager.h"
#include "AxonMQTTClient.h"
#include "AxonVPIN.h"
#include "AxonLogger.h"

class AxonSDK {
public:
    AxonSDK();
    ~AxonSDK();

    // Initialization
    void begin(const char* deviceToken, const char* deviceSecret, Client* customClient = nullptr);
    
    // Blynk-style macro begin wrapper
    void begin(const char* ssid, const char* password) {
#ifdef AXON_AUTH_TOKEN
        begin(AXON_AUTH_TOKEN, "", nullptr);
        connectWiFi(ssid, password);
        connect();
#endif
    }

    // Networking
    bool connectWiFi(const char* ssid, const char* password);
    bool connect(); // Connects to AXON Platform (MQTT)

    // Virtual Pins (VPIN) API
    void virtualWrite(const char* vpin, float value);
    void virtualWrite(const char* vpin, const char* value);
    void virtualWrite(const char* vpin, int value);
    
    float virtualReadFloat(const char* vpin);
    int virtualReadInt(const char* vpin);
    const char* virtualReadString(const char* vpin);

    // Write Handler Registry
    typedef void (*AxonWriteHandler)(const char* value);
    void setWriteHandler(const char* vpin, AxonWriteHandler handler);
    void handleWrite(const char* vpin, const char* value);

    // Event Loop
    void run();

    // Logging & Telemetry
    void publishTelemetry(); // Batches and sends telemetry data
    void log(const char* message);

private:
    const char* _deviceToken;
    const char* _deviceSecret;

    // Write handlers table
#define AXON_MAX_PIN_HANDLERS 32
    struct AxonWriteHandlerEntry {
        const char* vpin;
        AxonWriteHandler handler;
    };
    AxonWriteHandlerEntry _writeHandlers[AXON_MAX_PIN_HANDLERS];
    int _writeHandlerCount;

    // Internal Managers
    AxonWiFiManager _wifiManager;
    AxonMQTTClient _mqttClient;
    AxonVPINManager _vpinManager;
};

extern AxonSDK Axon;

#endif // AXON_SDK_H
