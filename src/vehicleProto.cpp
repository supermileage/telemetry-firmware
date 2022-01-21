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
LoggingCommand<SensorEcu, String> ecuEct(&ecu, "PROTO-ECT", &SensorEcu::getECT, 1);
LoggingCommand<SensorEcu, String> ecuIat(&ecu, "PROTO-IAT", &SensorEcu::getIAT, 2);
LoggingCommand<SensorEcu, String> ecuRpm(&ecu, "PROTO-RPM", &SensorEcu::getRPM, 3);
LoggingCommand<SensorEcu, String> ecuUbAdc(&ecu, "PROTO-UBADC", &SensorEcu::getUbAdc, 4);
LoggingCommand<SensorEcu, String> ecu02S(&ecu, "PROTO-O2S", &SensorEcu::getO2S, 5);
LoggingCommand<SensorEcu, String> ecuSpark(&ecu, "PROTO-SPARK", &SensorEcu::getSpark, 6);
LoggingCommand<SensorGps, String> gpsLat(&gps, "PROTO-Latitude", &SensorGps::getLatitude, 7);
LoggingCommand<SensorGps, String> gpsHvel(&gps, "PROTO-Speed", &SensorGps::getHorizontalSpeed, 8);

// Array Definitions - MUST BE NULL TERMINATED
IntervalCommand *commands[] = { &ecuEct, &ecuIat, &ecuRpm, &ecuUbAdc, &ecu02S, &ecuSpark, &gpsLat, &gpsHvel, NULL};

String publishName = "BQIngestion";

// CurrrentVehicle namespace definitions
LoggingDispatcher* CurrentVehicle::buildLoggingDispatcher() {
    LoggingDispatcherBuilder builder(commands, &dataQ, publishName);
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
    // Engine Computer
    DEBUG_SERIAL("ECU RPM: " + ecu.getRPM() + " - ");
    DEBUG_SERIAL("ECU MAP: " + ecu.getMap() + " kPa - ");
    DEBUG_SERIAL("ECU TPS: " + ecu.getTPS() + " % - ");
    DEBUG_SERIAL("ECU Coolant Temp: " + ecu.getECT() + "°C - ");
    DEBUG_SERIAL("ECU Intake Temp: " + ecu.getIAT() + "°C - ");
    DEBUG_SERIAL("ECU O2 Sensor: " + ecu.getO2S() + " V - ");
    DEBUG_SERIAL("ECU Spark Advance: " + ecu.getSpark() + "° - ");
    DEBUG_SERIAL("ECU Fuel PWM 1: " + ecu.getFuelPW1() + " ms - ");
    DEBUG_SERIAL("ECU Fuel PWM 2: " + ecu.getFuelPW2() + " ms - ");
    DEBUG_SERIAL_LN("ECU Input Voltage: " + ecu.getUbAdc() + " v");
    DEBUG_SERIAL_LN();
}

bool CurrentVehicle::getTimeValid() {
    return gps.getTimeValid();

}

uint32_t CurrentVehicle::getUnixTime() {
    return gps.getUnixTime();
}

#endif