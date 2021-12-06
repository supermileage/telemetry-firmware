#include "TimeLib.h"

TimeLib::TimeLib(void (*timeValidCallback)()) {
    _timeValidCallback = timeValidCallback;
}

TimeLib::~TimeLib(){}

void TimeLib::begin() {}

void TimeLib:: handle() {

    if(!Time.isValid() && millis() - _lastInvalidRun >= INVALID_CHECKTIME) { 
        if(CurrentVehicle::getTimeValid()) {
            Time.setTime( (time_t) CurrentVehicle::getUnixTime()); 
        }
        _lastInvalidRun = millis();
    } 

    if(Time.isValid()) {     
        if(millis() - _lastValidRun >= REG_CHECKTIME) {
            if(CurrentVehicle::getTimeValid()) {
                Time.setTime( (time_t) CurrentVehicle::getUnixTime());
                _lastValidRun = millis(); 
            }
            else {
                Particle.syncTime(); 
            }
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

void TimeLib::_triggerTimeValidCallback() {
    _timeValidCallback();
}