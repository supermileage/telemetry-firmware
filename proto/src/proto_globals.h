#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "Particle.h"
#include "settings.h"
#include "Led.h"

#include "Sensor.h"
#include "SensorGps.h"
#include "SensorThermo.h"
#include "SensorEcu.h"

#include "DataQueue.h"
#include "LogCommand.h"
#include "DispatcherBuilder.h"

#define VEHICLE_NAME "proto"

#define NUM_SENSORS 4
#define NUM_LOG_COMMANDS 8

extern DataQueue dataQ;

extern SensorGps gps;
extern SensorThermo thermo1;
extern SensorThermo thermo2;
extern SensorEcu ecu;

extern Sensor *sensors[NUM_SENSORS];
extern IntervalCommand *commands[NUM_LOG_COMMANDS];

namespace SerialDebugPublishing {
    void publishMessage();

    void setupMessage();
}

#endif