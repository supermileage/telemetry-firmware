#ifndef _FC_GLOBALS_H_
#define _FC_GLOBALS_H_

#include "Particle.h"
#include "settings.h"
#include "Led.h"

#include "Sensor.h"
#include "SensorGps.h"
#include "SensorThermo.h"
#include "SensorSigStrength.h"
#include "SensorVoltage.h"

#include "DataQueue.h"
#include "SensorCommand.h"
#include "DispatcherBuilder.h"

#define VEHICLE_NAME "fc"

extern DataQueue dataQ;
extern SensorGps gps;

extern Sensor *sensors[];
extern IntervalCommand *commands[];
extern uint16_t sensor_count;
extern uint16_t command_count;

namespace CurrentVehicle {

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