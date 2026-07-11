#ifndef AXON_H
#define AXON_H

#include "AxonSDK.h"

#ifdef AXON_USE_CELLULAR
  #include "AxonCellular.h"
#endif

#ifdef AXON_USE_ESPNOW
  #include "AxonESPNow.h"
#endif

// Parameter parser class mimicking BlynkParam
class AxonParam {
public:
    AxonParam(const char* val) : _val(val) {}
    
    int asInt() const { 
        return _val ? atoi(_val) : 0; 
    }
    
    float asFloat() const { 
        return _val ? (float)atof(_val) : 0.0f; 
    }
    
    const char* asStr() const { 
        return _val ? _val : ""; 
    }
    
    const char* asString() const { 
        return asStr(); 
    }

private:
    const char* _val;
};

// Blynk-style AXON_WRITE callback macro
#define AXON_WRITE(vpin) \
    void AxonWriteHandler_ ## vpin (const AxonParam& param); \
    static void AxonWriteGlue_ ## vpin (const char* val) { \
        AxonParam param(val); \
        AxonWriteHandler_ ## vpin (param); \
    } \
    struct AxonWriteHelper_ ## vpin { \
        AxonWriteHelper_ ## vpin() { \
            Axon.setWriteHandler(#vpin, AxonWriteGlue_ ## vpin); \
        } \
    } axon_write_helper_ ## vpin; \
    void AxonWriteHandler_ ## vpin (const AxonParam& param)

#endif // AXON_H
