#ifndef _TELEMETRY_H
#define _TELEMETRY_H

#include "Particle.h"
#include "settings.h"

#include "DataQueue.h"
#include "Led.h"
#include "Sensor.h"
#include "SensorGps.h"
#include "SensorThermo.h"

#if OUTPUT_SERIAL_MSG
    #define DEBUG_SERIAL(x) Serial.println(x)
#else
    #define DEBUG_SERIAL(x)
#endif

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

#endif