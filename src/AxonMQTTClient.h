#ifndef AXON_MQTT_CLIENT_H
#define AXON_MQTT_CLIENT_H

#include <Arduino.h>
#include <Client.h>
#include <PubSubClient.h>

#if defined(ESP32)
#include <WiFiClient.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

// We use the standard PubSubClient callback signature
typedef void (*MqttCallback)(char*, uint8_t*, unsigned int);

class AxonMQTTClient {
public:
    AxonMQTTClient(Client* customClient = nullptr);
    ~AxonMQTTClient();

    void begin(const char* token, const char* secret);
    bool connect();
    void maintain(); // Reconnects if dropped, calls loop()
    bool isConnected();
    
    bool publish(const char* topic, const char* payload);
    void setCallback(MqttCallback callback);
    void setClient(Client* client);

private:
    WiFiClient _wifiClient;
    PubSubClient* _mqtt;
    const char* _token;
    const char* _secret;
    unsigned long _lastReconnectAttempt;

    // Hardcoded to Fly.io broker for now, could be dynamic later
    const char* _brokerHost = "66.241.124.231";
    const int _brokerPort = 1883; 
};

#endif
