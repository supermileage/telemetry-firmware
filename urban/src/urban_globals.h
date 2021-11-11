#ifndef _URBAN_GLOBALS_H_
#define _URBAN_GLOBALS_H_

#include "Particle.h"
#include "settings.h"
#include "Led.h"

#include "SensorGps.h"
#include "SensorThermo.h"
#include "SensorCan.h"

#include "DataQueue.h"
#include "SensorCommand.h"
#include "DispatcherBuilder.h"

#define VEHICLE_NAME "urban"

extern DataQueue dataQ;
extern SensorGps gps;

extern Sensor *sensors[];
extern IntervalCommand *commands[];
extern uint16_t sensor_count;
extern uint16_t command_count;

namespace CurrentVehicle {
    
    /**
     * Publish message for urban
    **/
    void publishMessage();

    /**
     * Prints urban's setup message
    **/
    void setupMessage();
}

#endif