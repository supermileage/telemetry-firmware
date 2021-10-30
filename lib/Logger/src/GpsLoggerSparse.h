#ifndef _GPS_LOGGER_SPARSE_H_
#define _GPS_LOGGER_SPARSE_H_

#include "GpsLogger.h"
#include "DataQueue.h"

class GpsLoggerSparse : public GpsLogger {
    public:
        GpsLoggerSparse(SensorGps *sensor) : GpsLogger(sensor) { }

        void execute(DataQueue *dataQ) {
            dataQ->add(HorizontalAccuracy, _sensor->getHorizontalAccuracy());
            dataQ->add(VerticalAccuracy, _sensor->getVerticalAccuracy());
        }
};

#endif