#include "TimeLib.h"

TimeLib::TimeLib() {
}

void TimeLib:: handle() {
    if(!Time.isValid() && millis() - lastInvalidRun >= INVALID_CHECKTIME) { 
        if(CurrentVehicle::getTimeValid()) {
            Time.setTime( (time_t) CurrentVehicle::getUnixTime()); 
        }
        lastInvalidRun = millis();
    } 
    if(Time.isValid()) {     
        if(millis() - lastValidRun >= REG_CHECKTIME) {
            Time.setTime( (time_t) CurrentVehicle::getUnixTime());
            lastValidRun = millis();
        }
    }
}
