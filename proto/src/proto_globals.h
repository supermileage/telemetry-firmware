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

extern DataQueue dataQ;

extern SensorGps gps;

extern Sensor *sensors[];
extern IntervalCommand *commands[];
extern uint16_t sensor_count;
extern uint16_t command_count;

namespace SerialDebugPublishing {
    void publishMessage();

    void setupMessage();
}

#endif