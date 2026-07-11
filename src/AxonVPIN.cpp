#include "AxonVPIN.h"
#include <string.h>
#include <stdio.h>

AxonVPINManager::AxonVPINManager() : _pinCount(0) {
    memset(_pins, 0, sizeof(_pins));
}

AxonVPINManager::~AxonVPINManager() {}

int AxonVPINManager::findOrCreate(const char* vpin) {
    for (int i = 0; i < _pinCount; i++) {
        if (strcmp(_pins[i].name, vpin) == 0) {
            return i;
        }
    }
    if (_pinCount < MAX_VPINS) {
        strncpy(_pins[_pinCount].name, vpin, sizeof(_pins[_pinCount].name) - 1);
        _pinCount++;
        return _pinCount - 1;
    }
    return -1;
}

void AxonVPINManager::write(const char* vpin, float value) {
    int idx = findOrCreate(vpin);
    if (idx >= 0) {
        if (_pins[idx].type != 0 || _pins[idx].valueFloat != value) {
            _pins[idx].valueFloat = value;
            _pins[idx].type = 0;
            _pins[idx].updated = true;
        }
    }
}

void AxonVPINManager::write(const char* vpin, int value) {
    int idx = findOrCreate(vpin);
    if (idx >= 0) {
        if (_pins[idx].type != 1 || _pins[idx].valueInt != value) {
            _pins[idx].valueInt = value;
            _pins[idx].type = 1;
            _pins[idx].updated = true;
        }
    }
}

void AxonVPINManager::write(const char* vpin, const char* value) {
    int idx = findOrCreate(vpin);
    if (idx >= 0) {
        if (_pins[idx].type != 2 || strcmp(_pins[idx].valueString, value) != 0) {
            strncpy(_pins[idx].valueString, value, sizeof(_pins[idx].valueString) - 1);
            _pins[idx].type = 2;
            _pins[idx].updated = true;
        }
    }
}

float AxonVPINManager::readFloat(const char* vpin) {
    for (int i = 0; i < _pinCount; i++) {
        if (strcmp(_pins[i].name, vpin) == 0) return _pins[i].valueFloat;
    }
    return 0.0f;
}

int AxonVPINManager::readInt(const char* vpin) {
    for (int i = 0; i < _pinCount; i++) {
        if (strcmp(_pins[i].name, vpin) == 0) return _pins[i].valueInt;
    }
    return 0;
}

const char* AxonVPINManager::readString(const char* vpin) {
    for (int i = 0; i < _pinCount; i++) {
        if (strcmp(_pins[i].name, vpin) == 0) return _pins[i].valueString;
    }
    return "";
}

bool AxonVPINManager::hasUpdates() {
    for (int i = 0; i < _pinCount; i++) {
        if (_pins[i].updated) return true;
    }
    return false;
}

void AxonVPINManager::clearUpdates() {
    for (int i = 0; i < _pinCount; i++) {
        _pins[i].updated = false;
    }
}

bool AxonVPINManager::buildUpdateJson(char* buffer, size_t maxLen) {
    if (!hasUpdates()) return false;
    
    // Simple manual JSON string building to avoid huge dependencies if ArduinoJson isn't available
    strcpy(buffer, "{");
    bool first = true;
    
    for (int i = 0; i < _pinCount; i++) {
        if (_pins[i].updated) {
            if (!first) strcat(buffer, ",");
            first = false;
            
            char temp[128];
            if (_pins[i].type == 0) {
                snprintf(temp, sizeof(temp), "\"%s\":%.2f", _pins[i].name, _pins[i].valueFloat);
            } else if (_pins[i].type == 1) {
                snprintf(temp, sizeof(temp), "\"%s\":%d", _pins[i].name, _pins[i].valueInt);
            } else if (_pins[i].type == 2) {
                snprintf(temp, sizeof(temp), "\"%s\":\"%s\"", _pins[i].name, _pins[i].valueString);
            }
            strcat(buffer, temp);
        }
    }
    strcat(buffer, "}");
    return true;
}
