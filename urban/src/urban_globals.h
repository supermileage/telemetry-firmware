#ifndef _URBAN_GLOBALS_H_
#define _URBAN_GLOBALS_H_

#include "Particle.h"
#include "settings.h"
#include "Led.h"

#include "SensorGps.h"
#include "SensorThermo.h"
#include "SensorCan.h"

#include "DataQueue.h"
#include "DispatcherBuilder.h"

#define NUM_SENSORS 3
#define NUM_LOG_COMMANDS 3
#define VEHICLE_NAME "urban"

extern DataQueue dataQ;

extern SensorGps gps;
extern SensorThermo thermo1;
extern SensorThermo thermo2;

extern Sensor *sensors[NUM_SENSORS];
extern IntervalCommand *commands[NUM_LOG_COMMANDS];

namespace SerialDebugPublishing {
    void publishMessage();

    void setupMessage();
}

#endif