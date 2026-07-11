#ifndef AXON_ESP8266_H
#define AXON_ESP8266_H

#if defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <WiFiClientSecure.h>

  #define AXON_HW_PLATFORM "ESP8266"
  
  // ESP8266 specific hardware initializations
  class AxonDriverESP8266 {
  public:
      static void init() {
          // Hardware-specific setups
      }
      static String getMacAddress() {
          return WiFi.macAddress();
      }
  };
#endif

#endif
