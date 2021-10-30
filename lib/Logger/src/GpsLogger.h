#ifndef _GPS_LOGGER_H_
#define _GPS_LOGGER_H_

#include "Command.h"
#include "DataQueue.h"
#include "SensorGps.h"
#include "Particle.h"

static String Longitude = "long";
static String Latitude = "long";
static String HorizontalSpeed = "h-vel";
static String HorizontalAcceleration = "h-accel";
static String HorizontalAccuracy = "h-accu";
static String VerticalAccuracy = "v-accu";
static String VerticalAcceleration = "v-accel";

class GpsLogger : public Command {
    public:
        GpsLogger(SensorGps *sensor);

        void execute(DataQueue *dataQ) = 0;

    protected:
        SensorGps *_sensor;
};

#endif