#include "vehicle.h"

#ifdef FC

// sensor definitions
SensorGps gps(new SFE_UBLOX_GNSS());
SensorThermo thermo1(&SPI, A5);
SensorThermo thermo2(&SPI, A4);
SensorSigStrength sigStrength;
SensorVoltage inVoltage;

// command definitions
SensorCommand<SensorGps, String> gpsLat(&gps, "lat", &SensorGps::getLatitude, 1);
SensorCommand<SensorGps, String> gpsLong(&gps, "long", &SensorGps::getLongitude, 1);
SensorCommand<SensorGps, String> gpsVertAccel(&gps, "v-accel", &SensorGps::getVerticalAcceleration, 2);
SensorCommand<SensorGps, String> gpsHorAccel(&gps, "h-accel", &SensorGps::getHorizontalAcceleration, 2);
SensorCommand<SensorThermo, String> thermoTemp1(&thermo1, "temp1", &SensorThermo::getProbeTemp, 5);
SensorCommand<SensorThermo, String> thermoTemp2(&thermo2, "temp2", &SensorThermo::getProbeTemp, 5);

// Array Definitions - MUST BE NULL TERMINATED
IntervalCommand *commands[] = { &gpsLat, &gpsLong, &gpsVertAccel, &gpsHorAccel, &thermoTemp1, &thermoTemp2, NULL};

String publishName = "BQIngestion";

// CurrentVehicle namespace definitions
Dispatcher* CurrentVehicle::buildDispatcher() {
    DispatcherBuilder builder(commands, &dataQ, publishName);
    return builder.build();
}

void CurrentVehicle::debugSensorData() {
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

bool CurrentVehicle::getTimeValid() {
    return gps.getTimeValid();

}

uint32_t CurrentVehicle::getUnixTime() {
    return gps.getUnixTime();
}
 
#endif