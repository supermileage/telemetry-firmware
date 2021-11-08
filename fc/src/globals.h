#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "DispatcherFactory.h"
#include "DataQueue.h"
#include "Led.h"
#include "Particle.h"
#include "settings.h"
#include "SensorGps.h"
#include "SensorCan.h"
#include "SensorThermo.h"
#include "LogCommand.h"

#define NUM_SENSORS 3
#define NUM_LOG_COMMANDS 6

extern int six;
extern char theWord[128];
extern SensorGps gps1;

extern DataQueue dataQ;

extern SensorGps gps;
extern SensorThermo thermo1;
extern SensorThermo thermo2;

// extern LogCommand<SensorGps, float> gpsLat;
// extern LogCommand<SensorGps, float> gpsLong;
// extern LogCommand<SensorGps, float> gpsVertAccel;
// extern LogCommand<SensorGps, float> gpsHorAccel;
// extern LogCommand<SensorThermo, double> thermoTemp1;
// extern LogCommand<SensorThermo, double> thermoTemp2;

extern Sensor *sensors[NUM_SENSORS];
extern IntervalCommand *commands[NUM_LOG_COMMANDS];

#endif