#ifndef _TELEMETRY_H
#define _TELEMETRY_H

#include "Particle.h"
#include "settings.h"

#include "DataQueue.h"
#include "Sensor.h"
#include "SensorGps.h"
#include "SensorThermo.h"

#if OUTPUT_SERIAL_MSG
    #define DEBUG_SERIAL(x) Serial.println(x)
#else
    #define DEBUG_SERIAL(x)
#endif

DataQueue dataQ;

void newPayload(){
    dataQ.resetData();
}

void addMessage(String id, int value){
    dataQ.add(id, value);
}

void addMessage(String id, String value){
    dataQ.add(id, value);
}

void addMessage(String id, float value){
    dataQ.add(id, value);
}

void publishMessage(String topic) {
    DEBUG_SERIAL("------------------------");
    if(PUBLISH_ENABLED){
        DEBUG_SERIAL("Publish - ENABLED - Message: ");
        // Publish to Particle Cloud
        DEBUG_SERIAL(dataQ.publish(topic, PRIVATE, WITH_ACK));
    }else{
        DEBUG_SERIAL("Publish - DISABLED - Message: ");
        DEBUG_SERIAL(dataQ.resetData());
    }

    if(OUTPUT_FREE_MEM){
        DEBUG_SERIAL("\nFree RAM: " + String(System.freeMemory()) + "B / 80000B");
    }
}

#endif