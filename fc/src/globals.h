#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "Particle.h"
#include "settings.h"
#include "SensorGps.h"
#include "SensorCan.h"
#include "SensorThermo.h"
#include "LogCommand.h"

#define NUM_SENSORS 3
#define NUM_LOG_COMMANDS 6

extern DataQueue dataQ;

extern SensorGps gps;
extern SensorThermo thermo1;
extern SensorThermo thermo2;

extern Sensor *sensors[NUM_SENSORS];
extern IntervalCommand *commands[NUM_LOG_COMMANDS];

#endif