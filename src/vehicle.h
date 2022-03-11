#ifndef _VEH_H_
#define _VEH_H_

#include "settings.h"

#include "DataQueue.h"
#include "LoggingCommand.h"
#include "LoggingDispatcherBuilder.h"

#include "Sensor.h"
#include "SensorGps.h"
#include "SensorThermo.h"
#include "SensorSigStrength.h"
#include "SensorVoltage.h"

extern DataQueue dataQ;

namespace CurrentVehicle {

    /**
     *  Returns dispatcher with current vehicle's set of commands
    **/
    LoggingDispatcher* buildLoggingDispatcher();

    /**
     *  Output latest sensor data for debugging
    **/
    void debugSensorData();

    /**
     *  Output time validity from GPS sensor
    **/
    bool getTimeValid(); //returns time validity

    /**
     *  Output time data from GPS sensor
    **/
    uint32_t getUnixTime(); 

    /**
     * @brief Toggle override for GPS position greenlist
    **/
    void toggleGpsOverride();

    /**
     * @brief Send Restart message to TinyBMS
    **/
    void restartTinyBms();

}

#endif