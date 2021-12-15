#include "vehicle.h"

#ifdef URBAN

#include "SensorCan.h"

// sensor definitions
SensorGps gps(new SFE_UBLOX_GNSS());
SensorThermo thermo1(&SPI, A5);
SensorThermo thermo2(&SPI, A4);
SensorCan can(&SPI1, D5, D6);
SensorSigStrength sigStrength;
SensorVoltage inVoltage;

// command definitions
SensorCommand<SensorGps, String> gpsLat(&dataQ, &gps, "URBAN-Latitude", &SensorGps::getLatitude, 1);
SensorCommand<SensorGps, String> gpsLong(&dataQ, &gps, "URBAN-Longitude", &SensorGps::getLongitude, 1);
SensorCommand<SensorThermo, String> thermoTemp1(&dataQ, &thermo1, "URBAN-Temperature", &SensorThermo::getProbeTemp, 5);

// Array Definitions - MUST BE NULL TERMINATED
IntervalCommand *commands[] = { &gpsLat, &gpsLong, &thermoTemp1, NULL};

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
    // CAN
    for(int i = 0; i < can.getNumIds(); i++){
        String output = "CAN ID: 0x" + String(can.getId(i), HEX) + " - CAN Data:";
        uint8_t canDataLength = can.getDataLen(i);
        unsigned char* canData = can.getData(i);
        for(int k = 0; k < canDataLength; k++){
            output += " 0x";
            output += String(canData[k], HEX);
        }
        DEBUG_SERIAL_LN(output);
    }
}

bool CurrentVehicle::getTimeValid() {
    return gps.getTimeValid();

}

uint32_t CurrentVehicle::getUnixTime() {
    return gps.getUnixTime();
}

#endif