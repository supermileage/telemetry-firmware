#include "vehicle.h"

#ifdef URBAN

#include <map>
#include "CanInterface.h"
#include "CanListener.h"
#include "CanSensorAccessories.h"
#include "CanSensorTinyBms.h"

CanInterface canInterface(&SPI1, D5, D6);

// Sensor definitions
SensorGps gps(new SFE_UBLOX_GNSS());
SensorThermo thermo1(&SPI, A5);
SensorThermo thermo2(&SPI, A4); 
SensorSigStrength sigStrength;
SensorVoltage inVoltage;
CanSensorAccessories canSensorAccessories(canInterface, CAN_ACC_STATUS);
CanSensorTinyBms bms(canInterface, 25);

// Command definitions
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
LoggingCommand<SensorGps, String> gpsHorAccuracy(&gps, "haccu", &SensorGps::getHorizontalAccuracy, 10);
LoggingCommand<SensorGps, String> gpsVerAccuracy(&gps, "vaccu", &SensorGps::getVerticalAccuracy, 10);

LoggingCommand<SensorThermo, int> thermoMotor(&thermo1, "tmpmot", &SensorThermo::getProbeTemp, 5);
LoggingCommand<SensorThermo, int> thermoMotorController(&thermo2, "tmpmc", &SensorThermo::getProbeTemp, 5);

LoggingCommand<CanSensorTinyBms, String> bmsSoc(&bms, "soc", &CanSensorTinyBms::getSoc, 10);
LoggingCommand<CanSensorTinyBms, String> bmsVoltage(&bms, "bmsv", &CanSensorTinyBms::getBatteryVolt, 1);
LoggingCommand<CanSensorTinyBms, String> bmsCurrent(&bms, "bmsa", &CanSensorTinyBms::getBatteryCurrent, 1);
LoggingCommand<CanSensorTinyBms, String> bmsCellMax(&bms, "cmaxv", &CanSensorTinyBms::getMaxVolt, 5);
LoggingCommand<CanSensorTinyBms, String> bmsCellMin(&bms, "cminv", &CanSensorTinyBms::getMinVolt, 5);
LoggingCommand<CanSensorTinyBms, int> bmsStatus(&bms, "bmsstat", &CanSensorTinyBms::getStatusBms, 5);
LoggingCommand<CanSensorTinyBms, int> bmsTempInternal(&bms, "tmpbms", &CanSensorTinyBms::getTempBms, 5);
LoggingCommand<CanSensorTinyBms, int> bmsTempBatt1(&bms, "tmpbt1", &CanSensorTinyBms::getBatteryTemp1, 5);
LoggingCommand<CanSensorTinyBms, int> bmsTempBatt2(&bms, "tmpbt2", &CanSensorTinyBms::getBatteryTemp2, 5);
LoggingCommand<CanSensorTinyBms, int> bmsFault(&bms, "bmsf", &CanSensorTinyBms::getFault, 5);

LoggingCommand<CanSensorAccessories, int> urbanHeadlights(&canSensorAccessories, "lhd", &CanSensorAccessories::getStatusHeadlights, 5);
LoggingCommand<CanSensorAccessories, int> urbanBrakelights(&canSensorAccessories, "lbk", &CanSensorAccessories::getStatusBrakelights, 1);
LoggingCommand<CanSensorAccessories, int> urbanHorn(&canSensorAccessories, "horn", &CanSensorAccessories::getStatusHorn, 1);
LoggingCommand<CanSensorAccessories, int> urbanHazards(&canSensorAccessories, "lhd", &CanSensorAccessories::getStatusHazards, 1);
LoggingCommand<CanSensorAccessories, int> urbanRightSig(&canSensorAccessories, "ltr", &CanSensorAccessories::getStatusRightSignal, 1);
LoggingCommand<CanSensorAccessories, int> urbanLeftSig(&canSensorAccessories, "ltl", &CanSensorAccessories::getStatusLeftSignal, 1);
LoggingCommand<CanSensorAccessories, int> urbanWipers(&canSensorAccessories, "wipe", &CanSensorAccessories::getStatusWipers, 5);

String publishName = "BQIngestion";

void sendCanSpeed(float speed){
    CanMessage message = CAN_MESSAGE_NULL;
    message.id = CAN_TELEMETRY_GPS_SPEED;
    if (speed <= 70.0 && speed >= 0){
        message.data[0] = (uint8_t)(speed*3.6);
        message.dataLength = 1;
        canInterface.sendMessage(message);
    }
    else{
        message.data[0] = (uint8_t)(255);
        message.dataLength = 1;
        canInterface.sendMessage(message);
    }
    
}

LoggingDispatcher* CurrentVehicle::buildLoggingDispatcher() {
    // added here because because this function is called on startup
    gps.updateSpeedCallback(sendCanSpeed);

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
    DEBUG_SERIAL_LN("Motor Temp: " + String(thermo1.getProbeTemp()) + "°C");
    DEBUG_SERIAL_LN("Motor Controller Temp: " + String(thermo2.getProbeTemp()) + "°C");
    // BMS
    DEBUG_SERIAL("Battery Voltage: " + String(bms.getBatteryVolt()) + "v - ");
    DEBUG_SERIAL("Battery Current: " + String(bms.getBatteryCurrent()) + "A - ");
    DEBUG_SERIAL("Max Cell Voltage: " + String(bms.getMaxVolt()) + "v - ");
    DEBUG_SERIAL("Min Cell Voltage: " + String(bms.getMinVolt()) + "v - ");
    DEBUG_SERIAL("State of Charge: " + String(bms.getSoc()) + "% - ");
    DEBUG_SERIAL("BMS Status: " + bms.getStatusBmsString() + " - ");
    DEBUG_SERIAL("BMS Fault: " + BmsFault::toString(bms.getFault()) + " - ");
    DEBUG_SERIAL("BMS Temperature: " + String(bms.getTempBms()) + "°C - ");
    DEBUG_SERIAL("Battery Temperature 1: " + String(bms.getBatteryTemp1()) + "°C - ");
    DEBUG_SERIAL_LN("Battery Temperature 2: " + String(bms.getBatteryTemp2()) + "°C");
    // CAN Accessories
    DEBUG_SERIAL("Headlights: " + BOOL_TO_STRING(canSensorAccessories.getStatusHeadlights()) + " - ");
    DEBUG_SERIAL("Brakelights: " + BOOL_TO_STRING(canSensorAccessories.getStatusBrakelights()) + " - ");
    DEBUG_SERIAL("Horn: " + BOOL_TO_STRING(canSensorAccessories.getStatusHorn()) + " - ");
    DEBUG_SERIAL("Hazards: " + BOOL_TO_STRING(canSensorAccessories.getStatusHazards()) + " - ");
    DEBUG_SERIAL("Right Signal: " + BOOL_TO_STRING(canSensorAccessories.getStatusRightSignal()) + " - ");
    DEBUG_SERIAL("Left Signal: " + BOOL_TO_STRING(canSensorAccessories.getStatusLeftSignal()) + " - ");
    DEBUG_SERIAL_LN("Wipers: " + BOOL_TO_STRING(canSensorAccessories.getStatusWipers()));

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
    bms.restart();
}

#endif