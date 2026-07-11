#ifndef AXON_LOGGER_H
#define AXON_LOGGER_H

#include <Arduino.h>

class AxonLogger {
public:
    static void log(const char* level, const char* message);
    static void info(const char* message);
    static void error(const char* message);
    static void warn(const char* message);
};

#endif
