#include "vehicle.h"

#ifdef FC

// sensor definitions
SensorGps gps(new SFE_UBLOX_GNSS(), GPS_UPDATE_FREQUENCY);
SensorThermo thermo1(&SPI, A5);
SensorThermo thermo2(&SPI, A4);
SensorSigStrength sigStrength;
SensorVoltage inVoltage;

// command definitions
SensorCommand<SensorGps, String> gpsLat(&dataQ, &gps, "lat", &SensorGps::getLatitude, 1);
SensorCommand<SensorGps, String> gpsLong(&dataQ, &gps, "long", &SensorGps::getLongitude, 1);
SensorCommand<SensorGps, String> gpsVertAccel(&dataQ, &gps, "v-accel", &SensorGps::getVerticalAcceleration, 2);
SensorCommand<SensorGps, String> gpsHorAccel(&dataQ, &gps, "h-accel", &SensorGps::getHorizontalAcceleration, 2);
SensorCommand<SensorThermo, String> thermoTemp1(&dataQ, &thermo1, "temp1", &SensorThermo::getProbeTemp, 5);
SensorCommand<SensorThermo, String> thermoTemp2(&dataQ, &thermo2, "temp2", &SensorThermo::getProbeTemp, 5);

// array definitions: update counts if you make changes here
Sensor *sensors[] = {&gps, &thermo1, &thermo2, &sigStrength, &inVoltage};
IntervalCommand *commands[] = { &gpsLat, &gpsLong, &gpsVertAccel, &gpsHorAccel, &thermoTemp1, &thermoTemp2 };
uint16_t sensor_count = 5;
uint16_t command_count = 6;


// SerialDebugPublishing namespace definitions

void CurrentVehicle::debugSensorData() {
    DEBUG_SERIAL_LN();
    DEBUG_SERIAL_LN("SENSOR READINGS: ");
    // Diagnostic
    DEBUG_SERIAL("Signal Strength: " + sigStrength.getStrength() + " % - ");
    DEBUG_SERIAL("Signal Quality: " + sigStrength.getQuality() + " % - ");
    DEBUG_SERIAL_LN("Input Voltage: "+ String(inVoltage.getVoltage()) + " V");
    // Thermocouples
    DEBUG_SERIAL("Temperature (Thermo1): " + thermo1.getProbeTemp() + "°C - ");
    DEBUG_SERIAL("Temperature (Thermo2): " + thermo2.getProbeTemp() + "°C - ");
    DEBUG_SERIAL_LN("Internal Temperature (Thermo1): " + thermo1.getInternalTemp() + "°C");
    // GPS
    DEBUG_SERIAL("Longitude: " + gps.getLongitude() + "° - ");
    DEBUG_SERIAL("Latitude: " + gps.getLatitude() + "° - ");
    DEBUG_SERIAL("Horizontal Acceleration: " + gps.getHorizontalAcceleration() + " m/s^2 - ");
    DEBUG_SERIAL("Altitude: " + gps.getAltitude() + " m - ");
    DEBUG_SERIAL("Vertical Acceleration: " + gps.getHorizontalAcceleration() + " m/s^2 - ");
    DEBUG_SERIAL("Horizontal Accuracy: " + gps.getHorizontalAccuracy() + " m - ");
    DEBUG_SERIAL("Vertical Accuracy: " + gps.getVerticalAccuracy() + " m - ");  
    DEBUG_SERIAL_LN("Satellites in View: " + gps.getSatellitesInView());
    DEBUG_SERIAL_LN();
}

#endif