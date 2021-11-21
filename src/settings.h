#ifndef _SETTINGS_H_
#define _SETTINGS_H_

/**
 *  SETTINGS
**/
// If no argument is passed to compiler, allow us to manually define a vehicle
#if !defined(PROTO) && !defined(URBAN) && !defined(FC)
    // SELECT VEHICLE: PROTO URBAN FC 
    #define FC
#endif

// Logging enabled at boot-up
#define LOGGING_ENABLED_AT_BOOT 0
// Output Serial messages (disable for production)
#define DEBUG_SERIAL_ENABLE     1
// Output Sensor data over serial (disable for production)
#define DEBUG_SENSOR_ENABLE     1
// Output free memory to console
#define DEBUG_MEM               1

#define PUBLISH_INTERVAL_MS     5000
#define GPS_UPDATE_FREQUENCY    4

// Time zone in UTC offset (must manually adjust for DST)
#define TIME_ZONE                   -8

/**
 *  MACROS
**/
#if DEBUG_SERIAL_ENABLE
    #define DEBUG_SERIAL_LN(x) Serial.println(x)
    #define DEBUG_SERIAL(x) Serial.print(x)
#else
    #define DEBUG_SERIAL_LN(x) x
    #define DEBUG_SERIAL(x) x
#endif

#ifdef  PROTO
    #define VEHICLE_NAME "PROTO"
#elif   defined(URBAN)
    #define VEHICLE_NAME "URBAN"
#elif   defined(FC)
    #define VEHICLE_NAME "FC"
#endif


#endif