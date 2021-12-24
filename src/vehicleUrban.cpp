#include "vehicle.h"

#ifdef URBAN

#include <map>
#include "CanInterface.h"
#include "CanListener.h"
#include "CanListenerAccessories.h"

CanInterface canInterface(&SPI1, D5, D6);

// Sensor definitions
SensorGps gps(new SFE_UBLOX_GNSS());
SensorThermo thermo1(&SPI, A5);
SensorThermo thermo2(&SPI, A4);
SensorSigStrength sigStrength;
SensorVoltage inVoltage;
CanListenerAccessories canListenerAccessories(&canInterface, CAN_ACC_STATUS,
    { STATUS_HEADLIGHTS, STATUS_BRAKELIGHTS, STATUS_HORN, STATUS_HAZARDS,
    STATUS_RIGHT_SIGNAL, STATUS_LEFT_SIGNAL, STATUS_WIPERS, STATUS_NULL });

// Command definitions
SensorCommand<SensorGps, String> gpsLat(&gps, "URBAN-Latitude", &SensorGps::getLatitude, 1);
SensorCommand<SensorGps, String> gpsLong(&gps, "URBAN-Longitude", &SensorGps::getLongitude, 1);
SensorCommand<SensorThermo, String> thermoTemp1(&thermo1, "URBAN-Temperature", &SensorThermo::getProbeTemp, 5);
SensorCommand<CanListenerAccessories, String> urbanHeadlights(&canListenerAccessories, "u-hl", &CanListenerAccessories::getStatusHeadlights, 1);
SensorCommand<CanListenerAccessories, String> urbanBrakelights(&canListenerAccessories, "u-bl", &CanListenerAccessories::getStatusBrakelights, 1);
SensorCommand<CanListenerAccessories, String> urbanHorn(&canListenerAccessories, "u-hrn", &CanListenerAccessories::getStatusHorn, 1);
SensorCommand<CanListenerAccessories, String> urbanHazards(&canListenerAccessories, "u-haz", &CanListenerAccessories::getStatusHazards, 1);
SensorCommand<CanListenerAccessories, String> urbanRightSig(&canListenerAccessories, "u-rsig", &CanListenerAccessories::getStatusRightSignal, 1);
SensorCommand<CanListenerAccessories, String> urbanLeftSig(&canListenerAccessories, "u-lsig", &CanListenerAccessories::getStatusLeftSignal, 1);
SensorCommand<CanListenerAccessories, String> urbanWipers(&canListenerAccessories, "u-wpr", &CanListenerAccessories::getStatusWipers, 1);

// Array Definitions - MUST BE NULL TERMINATED
IntervalCommand *commands[] = { &gpsLat, &gpsLong, &thermoTemp1, &urbanHeadlights, &urbanBrakelights, &urbanHorn, &urbanHazards, &urbanRightSig,
    &urbanLeftSig, &urbanWipers, NULL};

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
    DEBUG_SERIAL_LN("");
}

bool CurrentVehicle::getTimeValid() {
    return gps.getTimeValid();

}

uint32_t CurrentVehicle::getUnixTime() {
    return gps.getUnixTime();
}

#endif