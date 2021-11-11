#ifndef _SETTINGS_H
#define _SETTINGS_H

// Publish to Cloud (Disable when not necessary during dev to save data)
#define PUBLISH_ENABLED             0
// Output Serial messages (disable for production)
#define DEBUG_SERIAL_ENABLE         1
// Output Sensor data over serial (disable for production
#define DEBUG_SENSOR_ENABLE         1
// Log and output CPU time for each sensor poll and new message
#define DEBUG_CPU_TIME              0
// Output free memory to console
#define DEBUG_MEM                   1

#define PUBLISH_INTERVAL_MS         5000
#define GPS_UPDATE_FREQUENCY        4

// Time zone in UTC offset (must manually adjust for DST)
#define TIME_ZONE                   -7

#if DEBUG_SERIAL_ENABLE
    #define DEBUG_SERIAL_LN(x) Serial.println(x)
    #define DEBUG_SERIAL(x) Serial.print(x)
#else
    #define DEBUG_SERIAL_LN(x) x
    #define DEBUG_SERIAL(x) x
#endif

#endif