#ifndef _GPS_LOGGER_FREQUENT_H_
#define _GPS_LOGGER_FREQUENT_H_

#include "GpsLogger.h"
#include "DataQueue.h"

class GpsLoggerFrequent : public GpsLogger {
    public:
        GpsLoggerFrequent(SensorGps *sensor) : GpsLogger(sensor) { }

        void execute(DataQueue *dataQ) {
            dataQ->add(Longitude, _sensor->getLongitude());
            dataQ->add(Latitude, _sensor->getLatitude());
            dataQ->add(HorizontalSpeed, _sensor->getHorizontalSpeed());
            dataQ->add(HorizontalAcceleration, _sensor->getHorizontalAcceleration());
            dataQ->add(VerticalAcceleration, _sensor->getVerticalAcceleration());
        }
};

#endif