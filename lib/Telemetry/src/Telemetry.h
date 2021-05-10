#ifndef _TELEMETRY_H
#define _TELEMETRY_H

#include "Particle.h"
#include "settings.h"

#include "JsonMaker.h"
#include "Sensor.h"
#include "SensorGps.h"
#include "SensorThermo.h"



#if OUTPUT_SERIAL_MSG
    #define DEBUG_SERIAL(x) Serial.println(x)
#else
    #define DEBUG_SERIAL(x)
#endif

void publishMessage(String topic, String& msg) {
    if(PUBLISH_ENABLED){
        // Publish to Particle Cloud
        Particle.publish(topic, msg, PRIVATE, WITH_ACK);
        DEBUG_SERIAL("Publish - ENABLED - Message: ");
    }else{
        DEBUG_SERIAL("Publish - DISABLED - Message: ");
    }
    DEBUG_SERIAL("New JSON Message: " + msg);
}

#endif