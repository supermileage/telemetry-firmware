#ifndef _FC_GLOBALS_H_
#define _FC_GLOBALS_H_

#include "Particle.h"
#include "settings.h"
#include "Led.h"

#include "SensorGps.h"
#include "SensorCan.h"
#include "SensorThermo.h"

#include "DataQueue.h"
#include "LogCommand.h"
#include "DispatcherBuilder.h"

#define VEHICLE_NAME "fc"

extern DataQueue dataQ;
extern SensorGps gps;

extern Sensor *sensors[];
extern IntervalCommand *commands[];
extern uint16_t sensor_count;
extern uint16_t command_count;

namespace SerialDebugPublishing {

    /**
     * Publishes a new message to Particle Cloud for fc
    **/
    void publishMessage();

    /**
     * Prints fc's setup message
    **/
    void setupMessage();
}

#endif