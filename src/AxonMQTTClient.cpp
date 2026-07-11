#include "AxonMQTTClient.h"

AxonMQTTClient::AxonMQTTClient(Client* customClient) : _token(nullptr), _secret(nullptr), _lastReconnectAttempt(0) {
    _mqtt = new PubSubClient(customClient ? *customClient : _wifiClient);
}

AxonMQTTClient::~AxonMQTTClient() {
    delete _mqtt;
}

void AxonMQTTClient::begin(const char* token, const char* secret) {
    _token = token;
    _secret = secret;
    _mqtt->setServer(_brokerHost, _brokerPort);
}

bool AxonMQTTClient::connect() {
    if (_mqtt->connected()) {
        return true;
    }

    Serial.print("[AXON MQTT] Connecting to ");
    Serial.print(_brokerHost);
    Serial.println("...");

    // We use the Device Token as the Client ID, and Token/Secret as Username/Password
    if (_mqtt->connect(_token, _token, _secret)) {
        Serial.println("[AXON MQTT] Connected successfully!");
        
        // Subscribe to incoming commands specific to this device
        char cmdTopic[128];
        snprintf(cmdTopic, sizeof(cmdTopic), "devices/%s/commands/#", _token);
        _mqtt->subscribe(cmdTopic);
        Serial.print("[AXON MQTT] Subscribed to ");
        Serial.println(cmdTopic);
        
        return true;
    } else {
        Serial.print("[AXON MQTT] Failed, rc=");
        Serial.println(_mqtt->state());
        return false;
    }
}

void AxonMQTTClient::maintain() {
    if (!_mqtt->connected()) {
        unsigned long now = millis();
        if (now - _lastReconnectAttempt > 5000) {
            _lastReconnectAttempt = now;
            connect();
        }
    } else {
        _mqtt->loop();
    }
}

bool AxonMQTTClient::isConnected() {
    return _mqtt->connected();
}

bool AxonMQTTClient::publish(const char* topic, const char* payload) {
    if (!isConnected()) return false;
    return _mqtt->publish(topic, payload);
}

void AxonMQTTClient::setCallback(MqttCallback callback) {
    _mqtt->setCallback(callback);
}

void AxonMQTTClient::setClient(Client* client) {
    if (_mqtt) delete _mqtt;
    _mqtt = new PubSubClient(client ? *client : _wifiClient);
    _mqtt->setServer(_brokerHost, _brokerPort);
}
