#include "TimeLib.h"

TimeLib::TimeLib(void (*timeValidCallback)()) {
    _timeValidCallback = timeValidCallback;
}

TimeLib::~TimeLib(){}

void TimeLib::begin() {
    _initialized = true;
}

void TimeLib:: handle() {

    // If time is invalid, check GPS time at a set interval
    if(!Time.isValid() && millis() - _lastInvalidRun >= INVALID_CHECKTIME) { 
        if(CurrentVehicle::getTimeValid()) {
            Time.setTime( (time_t) CurrentVehicle::getUnixTime()); 
        }
        _lastInvalidRun = millis();
    } 

    // If time is valid, check GPS time or cell time at a set interval
    if(Time.isValid()) {     
        if(millis() - _lastValidRun >= REG_CHECKTIME) {
            if(CurrentVehicle::getTimeValid()) {
                Time.setTime( (time_t) CurrentVehicle::getUnixTime());
            } else {
                Particle.syncTime(); 
            }

            _lastValidRun = millis(); 
        }

        if(!_timeIsValid) {
            _timeIsValid = TRUE;
            _timeValidCallback();
        }
    }

}

// Wrapper for checking and getting time 
String TimeLib::getTimeString(){
    if(Time.isValid()){
        return Time.timeStr();
    }else{
        return "TIME NOT VALID";
    }
}
