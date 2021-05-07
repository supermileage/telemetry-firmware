#include "Particle.h"
#include "JsonMaker.h"
#include "Sensor.h"
#include "SensorGps.h"
#include "SensorThermo.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

// Publish to Cloud (Disable when not necessary during dev to save data)
#define PUBLISH_ENABLED             0
// Output Serial messages (disable for production)
#define OUTPUT_SERIAL_MSG           1
// Log and output delay for each sensor poll and new message
#define LOG_TIMING                  1

#if OUTPUT_SERIAL_MSG
    #define DEBUG_SERIAL(x) Serial.println(x)
#else
    #define DEBUG_SERIAL(x)
#endif

#define PUBLISH_INTERVAL_MS         5000
#define GPS_UPDATE_INTERVAL_MS      1000
#define THERMO_UPDATE_INTERVAL_MS   500

JsonMaker jsonMaker;
SensorGps gps(GPS_UPDATE_INTERVAL_MS);
SensorThermo thermoA(&SPI, A5, THERMO_UPDATE_INTERVAL_MS);

void publishMessage(String topic) {
    if(PUBLISH_ENABLED){
        // Publish to Particle Cloud
        Particle.publish(topic, jsonMaker.get(), PRIVATE, WITH_ACK);
        DEBUG_SERIAL("Publish - ENABLED - Message: ");
    }else{
        DEBUG_SERIAL("Publish - DISABLED - Message: ");
    }
    DEBUG_SERIAL("New JSON Message: " + jsonMaker.get());
}