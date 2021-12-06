#ifndef _TIME_LIB_
#define _TIME_LIB_

#include "vehicle.h"
#include "Handleable.h"

#define INVALID_CHECKTIME 1000
#define REG_CHECKTIME 60000

class TimeLib : public Handleable {
    public:

        /**
         * Constructor 
        **/
        TimeLib(void (*timeValidCallback)());
        ~TimeLib();

        void begin();
        
        /**
         * @return checks and updates based on time validity
         **/
        void handle();

        void setTimeValidCallback(void (*callback)());

        String getTimeString();

    private:

        bool _timeIsValid = false;
        unsigned long _lastValidRun = 0;
        unsigned long _lastInvalidRun = 0;
        void (*_timeValidCallback)();

        void _triggerTimeValidCallback();
};

#endif