#include "Sensor.h"

void Sensor::benchmarkedHandle() {
    long start = micros();
    this-> handle();
    long handleTime = micros() - start;
    if(handleTime > _longestHandleTime) _longestHandleTime = handleTime;
}


long Sensor::getLongestHandleTime() {
    long longestHandleTime = _longestHandleTime;
    _longestHandleTime = 0;
    return longestHandleTime;
}