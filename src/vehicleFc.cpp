#include "vehicle.h"

#ifdef FC

// sensor definitions
SensorGps gps(new SFE_UBLOX_GNSS());
SensorThermo thermo1(&SPI, A5);
SensorThermo thermo2(&SPI, A4);
SensorSigStrength sigStrength;
SensorVoltage inVoltage;

LoggingCommand<SensorSigStrength, int> signalStrength(&sigStrength, "sigstr", &SensorSigStrength::getStrength, 10);
LoggingCommand<SensorSigStrength, int> signalQuality(&sigStrength, "sigql", &SensorSigStrength::getQuality, 10);
LoggingCommand<SensorVoltage, String> voltage(&inVoltage, "vin", &SensorVoltage::getVoltage, 10);
LoggingCommand<SensorThermo, int> thermoInt(&thermo1, "tmpint", &SensorThermo::getInternalTemp, 10);

LoggingCommand<SensorGps, String> gpsLong(&gps, "lon", &SensorGps::getLongitude, 1);
LoggingCommand<SensorGps, String> gpsLat(&gps, "lat", &SensorGps::getLatitude, 1);
LoggingCommand<SensorGps, String> gpsHorSpeed(&gps, "hvel", &SensorGps::getHorizontalSpeed, 1);
LoggingCommand<SensorGps, String> gpsHorAccel(&gps, "hacce", &SensorGps::getHorizontalAcceleration, 1);
LoggingCommand<SensorGps, String> gpsVertAccel(&gps, "vacce", &SensorGps::getVerticalAcceleration, 1);
LoggingCommand<SensorGps, String> gpsHorAccuracy(&gps, "haccu", &SensorGps::getHorizontalAccuracy, 10);
LoggingCommand<SensorGps, String> gpsVerAccuracy(&gps, "vaccu", &SensorGps::getVerticalAccuracy, 10);

LoggingCommand<SensorThermo, int> thermoMotor(&thermo1, "tmpmot", &SensorThermo::getProbeTemp, 5);
LoggingCommand<SensorThermo, int> thermoFuelCell(&thermo2, "tmpfcs", &SensorThermo::getProbeTemp, 5);

// Array Definitions - MUST BE NULL TERMINATED
IntervalCommand *commands[] = { &signalStrength, &signalQuality, &voltage, &thermoInt, 
                                &gpsLong, &gpsLat, &gpsHorSpeed, &gpsHorAccel, &gpsVertAccel, &gpsHorAccuracy, &gpsVerAccuracy, &gpsHorAccuracy, &gpsVerAccuracy, 
                                &thermoMotor, &thermoFuelCell, 
                                NULL};

String publishName = "BQIngestion";

// CurrentVehicle namespace definitions
LoggingDispatcher* CurrentVehicle::buildLoggingDispatcher() {
    LoggingDispatcherBuilder builder(commands, &dataQ, publishName);
    return builder.build();
}

void CurrentVehicle::debugSensorData() {
    // Diagnostic
    DEBUG_SERIAL("Signal Strength: " + String(sigStrength.getStrength()) + "% - ");
    DEBUG_SERIAL("Signal Quality: " + String(sigStrength.getQuality()) + "% - ");
    DEBUG_SERIAL("Input Voltage: "+ String(inVoltage.getVoltage()) + "v - ");
    DEBUG_SERIAL_LN("Internal Temperature (Thermo1): " + String(thermo1.getInternalTemp()) + "°C");
    // GPS
    DEBUG_SERIAL("Longitude: " + gps.getLongitude() + "° - ");
    DEBUG_SERIAL("Latitude: " + gps.getLatitude() + "° - ");
    DEBUG_SERIAL("Horizontal Acceleration: " + gps.getHorizontalAcceleration() + "m/s^2 - ");
    DEBUG_SERIAL("Altitude: " + gps.getAltitude() + "m - ");
    DEBUG_SERIAL("Vertical Acceleration: " + gps.getHorizontalAcceleration() + "m/s^2 - ");
    DEBUG_SERIAL("Horizontal Accuracy: " + gps.getHorizontalAccuracy() + "m - ");
    DEBUG_SERIAL("Vertical Accuracy: " + gps.getVerticalAccuracy() + "m - ");
    DEBUG_SERIAL_LN("Satellites in View: " + String(gps.getSatellitesInView()));
    // Thermo
    DEBUG_SERIAL_LN("Motor Temp: " + String(thermo1.getProbeTemp()) + "°C");
    DEBUG_SERIAL_LN("Fuel Cell Temp: " + String(thermo2.getProbeTemp()) + "°C");

    DEBUG_SERIAL_LN();

}

bool CurrentVehicle::getTimeValid() {
    return gps.getTimeValid();

}

uint32_t CurrentVehicle::getUnixTime() {
    return gps.getUnixTime();
}
 
#endif