#include "vehicle.h"

#ifdef PROTO

#include "SensorEcu.h"

// sensor definitions
SensorGps gps(new SFE_UBLOX_GNSS());
SensorThermo thermo1(&SPI, A5);
SensorThermo thermo2(&SPI, A4);
SensorEcu ecu(&Serial1);
SensorSigStrength sigStrength;
SensorVoltage inVoltage;

// command definitions
LoggingCommand<SensorSigStrength, int> signalStrength(&sigStrength, "sigstr", &SensorSigStrength::getStrength, 10);
LoggingCommand<SensorSigStrength, int> signalQuality(&sigStrength, "sigql", &SensorSigStrength::getQuality, 10);
LoggingCommand<SensorVoltage, String> voltage(&inVoltage, "vin", &SensorVoltage::getVoltage, 10);
LoggingCommand<SensorThermo, int> thermoInt(&thermo1, "tmpint", &SensorThermo::getInternalTemp, 5);

LoggingCommand<SensorGps, String> gpsLong(&gps, "lon", &SensorGps::getLongitude, 1);
LoggingCommand<SensorGps, String> gpsLat(&gps, "lat", &SensorGps::getLatitude, 1);
LoggingCommand<SensorGps, int> gpsHeading(&gps, "hea", &SensorGps::getHeading, 1);
LoggingCommand<SensorGps, String> gpsAltitude(&gps, "alt", &SensorGps::getAltitude, 1);
LoggingCommand<SensorGps, String> gpsHorSpeed(&gps, "hvel", &SensorGps::getHorizontalSpeed, 1);
LoggingCommand<SensorGps, String> gpsHorAccel(&gps, "hacce", &SensorGps::getHorizontalAcceleration, 1);
LoggingCommand<SensorGps, String> gpsVertAccel(&gps, "vacce", &SensorGps::getVerticalAcceleration, 1);
LoggingCommand<SensorGps, String> gpsIncline(&gps, "incl", &SensorGps::getIncline, 1);
LoggingCommand<SensorGps, String> gpsHorAccuracy(&gps, "haccu", &SensorGps::getHorizontalAccuracy, 10);
LoggingCommand<SensorGps, String> gpsVerAccuracy(&gps, "vaccu", &SensorGps::getVerticalAccuracy, 10);

LoggingCommand<SensorThermo, int> thermoHead(&thermo1, "tmphead", &SensorThermo::getProbeTemp, 5);
LoggingCommand<SensorThermo, int> thermoCrank(&thermo1, "tmpcnk", &SensorThermo::getProbeTemp, 5);

LoggingCommand<SensorEcu, int> ecuOn(&ecu, "eon", &SensorEcu::getOn, 1);
LoggingCommand<SensorEcu, int> ecuRpm(&ecu, "rpm", &SensorEcu::getRPM, 1);
LoggingCommand<SensorEcu, String> ecuMap(&ecu, "map", &SensorEcu::getMap, 1);
LoggingCommand<SensorEcu, int> ecuTps(&ecu, "tps", &SensorEcu::getTPS, 1);
LoggingCommand<SensorEcu, int> ecuEct(&ecu, "tmpblk", &SensorEcu::getECT, 5);
LoggingCommand<SensorEcu, int> ecuIat(&ecu, "tmpia", &SensorEcu::getIAT, 5);
LoggingCommand<SensorEcu, String> ecuO2s(&ecu, "o2s", &SensorEcu::getO2S, 1);
LoggingCommand<SensorEcu, int> ecuSpark(&ecu, "spar", &SensorEcu::getSpark, 1);
LoggingCommand<SensorEcu, String> ecuFuel(&ecu, "pw1", &SensorEcu::getFuelPW1, 1);

String publishName = "BQIngestion";

// CurrrentVehicle namespace definitions
LoggingDispatcher* CurrentVehicle::buildLoggingDispatcher() {
    LoggingDispatcherBuilder builder(&dataQ, publishName, IntervalCommand::getCommands());
    return builder.build();
}

void CurrentVehicle::debugSensorData() {
    // System
    DEBUG_SERIAL("Signal Strength: " + String(sigStrength.getStrength()) + "% - ");
    DEBUG_SERIAL("Signal Quality: " + String(sigStrength.getQuality()) + "% - ");
    DEBUG_SERIAL("Input Voltage: "+ String(inVoltage.getVoltage()) + "v - ");
    DEBUG_SERIAL_LN("Internal Temperature (Thermo1): " + String(thermo1.getInternalTemp()) + "°C");
    // GPS
    DEBUG_SERIAL("Longitude: " + gps.getLongitude() + "° - ");
    DEBUG_SERIAL("Latitude: " + gps.getLatitude() + "° - ");
    DEBUG_SERIAL("Heading: " + String(gps.getHeading()) + "° - ");
    DEBUG_SERIAL("Altitude: " + gps.getAltitude() + "m - ");
    DEBUG_SERIAL("Horizontal Acceleration: " + gps.getHorizontalAcceleration() + "m/s^2 - ");
    DEBUG_SERIAL("Vertical Acceleration: " + gps.getHorizontalAcceleration() + "m/s^2 - ");
    DEBUG_SERIAL("Horizontal Accuracy: " + gps.getHorizontalAccuracy() + "m - ");
    DEBUG_SERIAL("Vertical Accuracy: " + gps.getVerticalAccuracy() + "m - ");
    DEBUG_SERIAL_LN("Satellites in View: " + String(gps.getSatellitesInView()));
    // Thermo
    DEBUG_SERIAL_LN("Engine Head Temp: " + String(thermo1.getProbeTemp()) + "°C");
    DEBUG_SERIAL_LN("Engine Crankcase Temp: " + String(thermo2.getProbeTemp()) + "°C");
    // Engine Computer
    DEBUG_SERIAL("ECU On: " + BOOL_TO_STRING(ecu.getOn()) + " - ");
    DEBUG_SERIAL("ECU RPM: " + String(ecu.getRPM()) + " - ");
    DEBUG_SERIAL("ECU MAP: " + ecu.getMap() + "kPa - ");
    DEBUG_SERIAL("ECU TPS: " + String(ecu.getTPS()) + "% - ");
    DEBUG_SERIAL("ECU Block Temp: " + String(ecu.getECT()) + "°C - ");
    DEBUG_SERIAL("ECU Intake Temp: " + String(ecu.getIAT()) + "°C - ");
    DEBUG_SERIAL("ECU O2 Sensor: " + ecu.getO2S() + "v - ");
    DEBUG_SERIAL("ECU Spark Advance: " + String(ecu.getSpark()) + "° - ");
    DEBUG_SERIAL_LN("ECU Fuel PWM 1: " + ecu.getFuelPW1() + "ms");

    DEBUG_SERIAL_LN();
}

bool CurrentVehicle::getTimeValid() {
    return gps.getTimeValid();

}

uint32_t CurrentVehicle::getUnixTime() {
    return gps.getUnixTime();
}

void CurrentVehicle::toggleGpsOverride() {
    gps.toggleOverride();
}

void CurrentVehicle::restartTinyBms() {
    
}

#endif