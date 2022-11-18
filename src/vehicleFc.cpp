#include "vehicle.h"
#include "SensorFcpCellStack.h"
#include "USARTSerialWrapper.h"
#include <vector>

#ifdef FC

// sensor definitions
SensorGps gps(new SFE_UBLOX_GNSS());
SensorThermo thermo1(&SPI, A5);
SensorThermo thermo2(&SPI, A4);
SensorSigStrength sigStrength;
SensorVoltage inVoltage;
USARTSerialWrapper serial(&Serial1);
SensorFcpCellStack cellStack(&serial);

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

LoggingCommand<SensorFcpCellStack, String> cellVoltage1(&cellStack, "cv1", &SensorFcpCellStack::getNextCellVoltage, 1);
LoggingCommand<SensorFcpCellStack, String> cellVoltage2(&cellStack, "cv2", &SensorFcpCellStack::getNextCellVoltage, 1);
LoggingCommand<SensorFcpCellStack, String> cellVoltage3(&cellStack, "cv3", &SensorFcpCellStack::getNextCellVoltage, 1);
LoggingCommand<SensorFcpCellStack, String> cellVoltage4(&cellStack, "cv4", &SensorFcpCellStack::getNextCellVoltage, 1);
LoggingCommand<SensorFcpCellStack, String> cellVoltage5(&cellStack, "cv5", &SensorFcpCellStack::getNextCellVoltage, 1);
LoggingCommand<SensorFcpCellStack, String> cellVoltage6(&cellStack, "cv6", &SensorFcpCellStack::getNextCellVoltage, 1);
LoggingCommand<SensorFcpCellStack, String> cellVoltage7(&cellStack, "cv7", &SensorFcpCellStack::getNextCellVoltage, 1);
LoggingCommand<SensorFcpCellStack, String> cellVoltage8(&cellStack, "cv8", &SensorFcpCellStack::getNextCellVoltage, 1);
LoggingCommand<SensorFcpCellStack, String> cellVoltage9(&cellStack, "cv9", &SensorFcpCellStack::getNextCellVoltage, 1);
LoggingCommand<SensorFcpCellStack, String> cellVoltage10(&cellStack, "cv10", &SensorFcpCellStack::getNextCellVoltage, 1);
LoggingCommand<SensorFcpCellStack, String> cellVoltage11(&cellStack, "cv11", &SensorFcpCellStack::getNextCellVoltage, 1);
LoggingCommand<SensorFcpCellStack, String> cellVoltage12(&cellStack, "cv12", &SensorFcpCellStack::getNextCellVoltage, 1);
LoggingCommand<SensorFcpCellStack, String> cellVoltage13(&cellStack, "cv13", &SensorFcpCellStack::getNextCellVoltage, 1);
LoggingCommand<SensorFcpCellStack, String> cellVoltage14(&cellStack, "cv14", &SensorFcpCellStack::getNextCellVoltage, 1);
LoggingCommand<SensorFcpCellStack, String> cellVoltage15(&cellStack, "cv15", &SensorFcpCellStack::getNextCellVoltage, 1);
LoggingCommand<SensorFcpCellStack, String> cellVoltage16(&cellStack, "cv16", &SensorFcpCellStack::getNextCellVoltage, 1);
LoggingCommand<SensorFcpCellStack, String> cellVoltage17(&cellStack, "cv17", &SensorFcpCellStack::getNextCellVoltage, 1);
LoggingCommand<SensorFcpCellStack, String> cellVoltage18(&cellStack, "cv18", &SensorFcpCellStack::getNextCellVoltage, 1);

LoggingCommand<SensorThermo, int> thermoMotor(&thermo1, "tmpmot", &SensorThermo::getProbeTemp, 5);
LoggingCommand<SensorThermo, int> thermoFuelCell(&thermo2, "tmpfcs", &SensorThermo::getProbeTemp, 5);

String publishName = "BQIngestion";

// CurrentVehicle namespace definitions
LoggingDispatcher* CurrentVehicle::buildLoggingDispatcher() {
    LoggingDispatcherBuilder builder(&dataQ, publishName, IntervalCommand::getCommands());
    return builder.build();
}

void CurrentVehicle::debugSensorData() {
    // Diagnostic
    DEBUG_SERIAL("Signal Strength: " + String(sigStrength.getStrength()) + "% - ");
    DEBUG_SERIAL("Signal Quality: " + String(sigStrength.getQuality()) + "% - ");
    DEBUG_SERIAL("Input Voltage: "+ String(inVoltage.getVoltage()) + "v - ");
    DEBUG_SERIAL_LN("Internal Temp: " + String(thermo1.getInternalTemp()) + "°C");
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
    DEBUG_SERIAL("Motor Temp: " + String(thermo1.getProbeTemp()) + "°C - ");
    DEBUG_SERIAL_LN("Fuel Cell Temp: " + String(thermo2.getProbeTemp()) + "°C");

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