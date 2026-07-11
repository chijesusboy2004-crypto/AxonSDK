#ifndef AXON_CELLULAR_H
#define AXON_CELLULAR_H

#include <Arduino.h>
#include "AxonSDK.h"

// TinyGSM is required for cellular support.
// User must define AXON_USE_CELLULAR and install TinyGSM library.
#ifdef AXON_USE_CELLULAR
#include <TinyGsmClient.h>

class AxonCellular {
public:
    AxonCellular(HardwareSerial& serialAT, int pwrKeyPin) 
        : _serialAT(serialAT), _pwrKeyPin(pwrKeyPin), _modem(serialAT), _client(_modem) {}

    bool begin(const char* apn, const char* username = "", const char* password = "") {
        Serial.println("[AXON Cellular] Powering modem...");
        pinMode(_pwrKeyPin, OUTPUT);
        digitalWrite(_pwrKeyPin, HIGH);
        delay(1000);
        digitalWrite(_pwrKeyPin, LOW);
        delay(3000);

        _serialAT.begin(115200, SERIAL_8N1);
        delay(2000);

        if (!_modem.restart()) {
            Serial.println("[AXON Cellular] Modem restart failed!");
            return false;
        }

        Serial.print("[AXON Cellular] Connecting to APN: ");
        Serial.println(apn);
        if (!_modem.gprsConnect(apn, username, password)) {
            Serial.println("[AXON Cellular] GPRS/LTE connection failed!");
            return false;
        }

        Serial.println("[AXON Cellular] Connected successfully!");
        
        // Auto-inject client into global Axon instance
#ifdef AXON_AUTH_TOKEN
        Axon.begin(AXON_AUTH_TOKEN, "", &_client);
#endif
        return true;
    }

    TinyGsm& getModem() { return _modem; }
    TinyGsmClient& getClient() { return _client; }

private:
    HardwareSerial& _serialAT;
    int _pwrKeyPin;
    TinyGsm _modem;
    TinyGsmClient _client;
};
#endif

#endif // AXON_CELLULAR_H
