#ifndef _TIME_LIB_
#define _TIME_LIB_

#include "vehicle.h"

#define INVALID_CHECKTIME 1000
#define REG_CHECKTIME 60000

class TimeLib
{
    public:

        /**
         * Constructor 
        **/
        TimeLib();

        /**
         * @return checks and updates based on time validity
         **/
        void handle();

    private:

        unsigned long lastValidRun = 0;
        unsigned long lastInvalidRun = 0;
};

#endif