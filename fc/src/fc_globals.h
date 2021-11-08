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

#define NUM_SENSORS 3
#define NUM_LOG_COMMANDS 6
#define VEHICLE_NAME "fc"

extern DataQueue dataQ;

extern SensorGps gps;
extern SensorThermo thermo1;
extern SensorThermo thermo2;

extern Sensor *sensors[NUM_SENSORS];
extern IntervalCommand *commands[NUM_LOG_COMMANDS];

namespace SerialDebugPublishing {

    /**
     * Publishes a new message to Particle Cloud
     * */
    void publishMessage();

    void setupMessage();
}

#endif