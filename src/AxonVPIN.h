#ifndef AXON_VPIN_H
#define AXON_VPIN_H

#include <Arduino.h>

#define MAX_VPINS 32

struct VPinEntry {
    char name[16];
    float valueFloat;
    int valueInt;
    char valueString[64];
    uint8_t type; // 0 = Float, 1 = Int, 2 = String
    bool updated;
};

class AxonVPINManager {
public:
    AxonVPINManager();
    ~AxonVPINManager();

    void write(const char* vpin, float value);
    void write(const char* vpin, int value);
    void write(const char* vpin, const char* value);

    float readFloat(const char* vpin);
    int readInt(const char* vpin);
    const char* readString(const char* vpin);

    // Used by SDK to batch and publish
    bool hasUpdates();
    void clearUpdates();
    
    // Builds JSON string of all updated pins. Returns true if string was built.
    bool buildUpdateJson(char* buffer, size_t maxLen);

private:
    VPinEntry _pins[MAX_VPINS];
    int _pinCount;

    int findOrCreate(const char* vpin);
};

#endif
