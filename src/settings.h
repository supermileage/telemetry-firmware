#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "Particle.h"
#include "DebugSerialOutput.h"

/**
 *  SETTINGS
**/
// If no argument is passed to compiler, allow us to manually define a vehicle
#if !defined(PROTO) && !defined(URBAN) && !defined(FC)
    // SELECT VEHICLE: PROTO URBAN FC 
    #define URBAN
#endif

// Important: Board version number
// TODO: remove when we have fully transitioned to second board
#define BOARD_V2

// Logging enabled at boot-up, control logging with button or Particle Function
#define LOGGING_EN_AT_BOOT      1
// Publish to Cloud
#define PUBLISH_EN              1
// Output Serial messages (disable for production)
#define DEBUG_SERIAL_EN         false
// Sensor Debug Interval in s, 0 for off
#define DEBUG_SENSOR_INT        5
// LED Flash interval in ms
#define LED_FLASH_INT           500
// Time zone in UTC offset (must manually adjust for DST)
#define TIME_ZONE               -7

/**
 *  MACROS
**/
#define DEBUG_SERIAL_LN(x) DebugSerial::instance().println(x)
#define DEBUG_SERIAL(x) DebugSerial::instance().print(x)
#define DEBUG_SERIAL_F(x, ...) DebugSerial::instance().printf(x, __VA_ARGS__)

#ifdef  PROTO
    #define VEHICLE_NAME "PROTO"
#elif   defined(URBAN)
    #define VEHICLE_NAME "URBAN"
#elif   defined(FC)
    #define VEHICLE_NAME "FC"
#elif   defined(SANDBOX)
    #define VEHICLE_NAME "SANDBOX"
#endif

#define FLOAT_TO_STRING(x,y) String(x,y)
#define BOOL_TO_STRING(x) String((x < 2) ? ((x == 1) ? "ON" : "OFF") : "N/A")

#endif