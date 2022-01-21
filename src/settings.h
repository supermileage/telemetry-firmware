#ifndef _SETTINGS_H_
#define _SETTINGS_H_

/**
 *  SETTINGS
**/
// If no argument is passed to compiler, allow us to manually define a vehicle
#if !defined(PROTO) && !defined(URBAN) && !defined(FC)
    // SELECT VEHICLE: PROTO URBAN FC 
    #define URBAN
#endif

// Logging enabled at boot-up, control logging with button or Particle Function
#define LOGGING_EN_AT_BOOT      0
// Publish to Cloud
#define PUBLISH_EN              1
// Output Serial messages (disable for production)
#define DEBUG_SERIAL_EN         1
// Sensor Debug Interval in s, 0 for off
#define DEBUG_SENSOR_INT        5
// LED Flash interval in ms
#define LED_FLASH_INT           500
// Time zone in UTC offset (must manually adjust for DST)
#define TIME_ZONE               -8

/**
 *  MACROS
**/
#if DEBUG_SERIAL_EN
    #define DEBUG_SERIAL_LN(x) Serial.println(x)
    #define DEBUG_SERIAL(x) Serial.print(x)
    #define DEBUG_SERIAL_F(x, ...) Serial.printf(x, __VA_ARGS__)
#else
    #define DEBUG_SERIAL_LN(x) x
    #define DEBUG_SERIAL(x) x
    #define DEBUG_SERIAL_F(X) x
#endif

#ifdef  PROTO
    #define VEHICLE_NAME "PROTO"
#elif   defined(URBAN)
    #define VEHICLE_NAME "URBAN"
#elif   defined(FC)
    #define VEHICLE_NAME "FC"
#endif

#endif