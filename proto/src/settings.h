#ifndef _SETTINGS_H
#define _SETTINGS_H

// Publish to Cloud (Disable when not necessary during dev to save data)
#define PUBLISH_ENABLED             1
// Output Serial messages (disable for production)
#define OUTPUT_SERIAL_MSG           1
// Log and output delay for each sensor poll and new message
#define LOG_TIMING                  0
// Output free memory to console
#define OUTPUT_FREE_MEM             1

#define PUBLISH_INTERVAL_MS         10000
#define GPS_UPDATE_INTERVAL_MS      1000
#define THERMO_UPDATE_INTERVAL_MS   500

// Time zone in UTC offset (must manually adjust for DST)
#define TIME_ZONE                   -7



#endif