#ifndef _PROTO_GLOBALS_H_
#define _PROTO_GLOBALS_H_

#include "Particle.h"
#include "settings.h"
#include "constants.h"
#include "Led.h"

#include "Sensor.h"
#include "SensorGps.h"
#include "SensorThermo.h"
#include "SensorEcu.h"
#include "SensorSigStrength.h"
#include "SensorVoltage.h"

#include "SensorCommand.h"
#include "DataQueue.h"
#include "DispatcherBuilder.h"

#define VEHICLE_NAME "proto"

extern DataQueue dataQ;
extern SensorGps gps;

extern Sensor *sensors[];
extern IntervalCommand *commands[];
extern uint16_t sensor_count;
extern uint16_t command_count;

namespace CurrentVehicle {
    void publishMessage();

    void setupMessage();
}

#endif