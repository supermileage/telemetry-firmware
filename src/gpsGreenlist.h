#ifndef _GPS_GREENLIST_H_
#define _GPS_GREENLIST_H_

#include <utility>

// This files defines GPS co-ordinate "boxes" where GPS position will be uploaded
// If the position is outside of one of these boxes, GPS position not uploaded for security
// IMPORTANT: Update this greenlist with any new areas used for testing/competitions

struct positionBox {
    double northLat;
    double southLat;
    double westLon;
    double eastLon;

    bool isWithin(double lon, double lat) {
        return lat <= northLat && lat >= southLat && lon >= westLon && lon <= eastLon;
    }
};

const positionBox GREEN_LIST[] = {  {/*N*/49.286, /*S*/49.239, /*W*/-123.281, /*E*/-123.222},   // UBC
                                    {/*N*/49.132, /*S*/49.120, /*W*/-122.339, /*E*/-122.307}};  // Mission Race Ways
                                    
// positionBox UBC(/*N*/49.286, /*S*/49.239, /*W*/-123.281, /*E*/-123.222);
// positionBox MissionRaceway(/*N*/49.132, /*S*/49.120, /*W*/-122.339, /*E*/-122.307);

#endif