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
            if(CurrentVehicle::getTimeValid()) {
                Time.setTime( (time_t) CurrentVehicle::getUnixTime());
                lastValidRun = millis(); 
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