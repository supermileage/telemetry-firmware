#ifndef _GPS_GREENLIST_H_
#define _GPS_GREENLIST_H_

#include <utility>

// This files defines GPS co-ordinate "boxes" where GPS position will be uploaded
// If the position is outside of one of these boxes, GPS position not uploaded for security
// IMPORTANT: Update this greenlist with any new areas used for testing/competitions

struct positionBox {
    const double northLat;
    const double southLat;
    const double westLon;
    const double eastLon;

    bool isWithin(double lon, double lat) {
        if(northLat < southLat || westLon > eastLon) {
            DEBUG_SERIAL_LN("ERROR: POSITION BOX INVALID!");
        }
        return lat <= northLat && lat >= southLat && lon >= westLon && lon <= eastLon;
    }
};

const positionBox GREEN_LIST[] = {  {/*N*/49.286, /*S*/49.239, /*W*/-123.281, /*E*/-123.222},   // UBC
                                    {/*N*/49.132, /*S*/49.120, /*W*/-122.339, /*E*/-122.307},   // Mission Race Ways
                                    {/*N*/39.809, /*S*/39.783, /*W*/-86.247, /*E*/-86.219}};    // Indianapolis Motor Speedway

#endif