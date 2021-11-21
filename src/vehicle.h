#ifndef _VEH_H_
#define _VEH_H_

#include "settings.h"

#include "DataQueue.h"
#include "SensorCommand.h"
#include "DispatcherBuilder.h"

#include "Sensor.h"
#include "SensorGps.h"
#include "SensorThermo.h"
#include "SensorSigStrength.h"
#include "SensorVoltage.h"

extern DataQueue dataQ;
extern SensorGps gps;

extern Sensor *sensors[];

namespace CurrentVehicle {

    /**
     *  Returns dispatcher with current vehicle's set of commands
    **/
    Dispatcher* buildDispatcher();

    /**
     *  Output latest sensor data for debugging
    **/
    void debugSensorData();
}

#endif