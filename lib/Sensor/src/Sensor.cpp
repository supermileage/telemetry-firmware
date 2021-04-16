#include "Sensor.h"

/**
 * Run handle function of sensor and record time elapsed if it's longer than longest time elapsed
 * */
void Sensor::benchmarkedHandle() {
    long start = micros();
    this-> handle();
    long handleTime = micros() - start;
    if(handleTime > _longestHandleTime) _longestHandleTime = handleTime;
}

/**
 * @return longest handle time since last checked
 * */
long Sensor::getLongestHandleTime() {
    long longestHandleTime = _longestHandleTime;
    _longestHandleTime = 0;
    return longestHandleTime;
}