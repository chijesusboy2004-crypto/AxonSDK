#ifndef AXON_ESP32_H
#define AXON_ESP32_H

#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <WiFiClientSecure.h>

  #define AXON_HW_PLATFORM "ESP32"
  
  // ESP32 specific hardware initializations
  class AxonDriverESP32 {
  public:
      static void init() {
          // Hardware-specific setups like watchdog timers
      }
      static String getMacAddress() {
          return WiFi.macAddress();
      }
  };
#endif

#endif
