#include "AxonLogger.h"

void AxonLogger::log(const char* level, const char* message) {
    Serial.print("[AXON ");
    Serial.print(level);
    Serial.print("] ");
    Serial.println(message);
    
    // In the future, this can also push logs over MQTT to the Digital Twin
}

void AxonLogger::info(const char* message) {
    log("INFO", message);
}

void AxonLogger::error(const char* message) {
    log("ERROR", message);
}

void AxonLogger::warn(const char* message) {
    log("WARN", message);
}
