#include "vehicle.h"

#ifdef URBAN

#include "Mcp2515CanWrapper.h"
#include "CanInterface.h"
#include "CanListener.h"
#include "CanSensorAccessories.h"
#include "CanSensorSteering.h"
#include "CanSensorTinyBms.h"
#include "CanSensorOrionBms.h"
#include "BmsManager.h"

// starting bms is arbitrary--will change after one or other starts receiving Can messages
#define DEFAULT_BMS BmsManager::BmsOption::Orion

Mcp2515CanWrapper canBus(&SPI1, D5, D6);
CanInterface canInterface((CanBus*)&canBus);

// Sensor definitions
SensorGps gps(new SFE_UBLOX_GNSS());
SensorThermo thermo1(&SPI, A5);
SensorThermo thermo2(&SPI, A4);
SensorSigStrength sigStrength;
SensorVoltage inVoltage;
CanSensorAccessories canSensorAccessories(canInterface, CAN_ACC_STATUS);
CanSensorSteering steering(canInterface);

// Bms
CanSensorTinyBms tinyBms(canInterface, 25);
CanSensorOrionBms orionBms(canInterface);
CanSensorBms* bms = DEFAULT_BMS == BmsManager::BmsOption::Orion ? (CanSensorBms*)(&orionBms) : (CanSensorBms*)(&tinyBms);
BmsManager bmsManager(&bms, &orionBms, &tinyBms, DEFAULT_BMS);

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
LoggingCommand<SensorGps, String> gpsIncline(&gps, "incl", &SensorGps::getIncline, 1);
LoggingCommand<SensorGps, String> gpsHorAccuracy(&gps, "haccu", &SensorGps::getHorizontalAccuracy, 10);
LoggingCommand<SensorGps, String> gpsVerAccuracy(&gps, "vaccu", &SensorGps::getVerticalAccuracy, 10);

LoggingCommand<SensorThermo, int> thermoMotor(&thermo1, "tmpmot", &SensorThermo::getProbeTemp, 5);
LoggingCommand<SensorThermo, int> thermoMotorController(&thermo2, "tmpmc", &SensorThermo::getProbeTemp, 5);

LoggingCommand<CanSensorSteering, int> steeringThrottle(&steering, "tps", &CanSensorSteering::getThrottle, 1);
LoggingCommand<CanSensorSteering, int> steeringIgnition(&steering, "ign", &CanSensorSteering::getIgnition, 1);
LoggingCommand<CanSensorSteering, int> steeringDms(&steering, "dms", &CanSensorSteering::getDms, 1);
LoggingCommand<CanSensorSteering, int> steeringBrake(&steering, "br", &CanSensorSteering::getBrake, 1);

LoggingCommand<CanSensorBms, String> bmsVoltage(bms, "bmsv", &CanSensorBms::getBatteryVolt, 1);
LoggingCommand<CanSensorBms, String> bmsCurrent(bms, "bmsa", &CanSensorBms::getBatteryCurrent, 1);
LoggingCommand<CanSensorBms, String> bmsCellMax(bms, "cmaxv", &CanSensorBms::getMaxVolt, 5);
LoggingCommand<CanSensorBms, String> bmsCellMin(bms, "cminv", &CanSensorBms::getMinVolt, 5);
LoggingCommand<CanSensorBms, String> bmsCellAvg(bms, "cavgv", &CanSensorBms::getAvgVolt, 5);
LoggingCommand<CanSensorBms, int> bmsStatus(bms, "bmsstat", &CanSensorBms::getStatusBms, 5);
LoggingCommand<CanSensorBms, int> bmsTempInternal(bms, "tmpbms", &CanSensorBms::getTempBms, 5);
LoggingCommand<CanSensorBms, int> bmsTempBatt1(bms, "tmpbt1", &CanSensorBms::getMaxBatteryTemp, 5);
LoggingCommand<CanSensorBms, int> bmsTempBatt2(bms, "tmpbt2", &CanSensorBms::getMinBatteryTemp, 5);
LoggingCommand<CanSensorBms, int> bmsCellTempAvg(bms, "tmpavg", &CanSensorBms::getAvgBatteryTemp, 5);
LoggingCommand<CanSensorBms, int> bmsFault(bms, "bmsf", &CanSensorBms::getFault, 5);
LoggingCommand<CanSensorBms, String> bmsSoc(bms, "soc", &CanSensorBms::getSoc, 10);
LoggingCommand<BmsManager, int> bmsType(&bmsManager, "bmst", &BmsManager::getCurrentBms, 10);

LoggingCommand<CanSensorAccessories, int> urbanHeadlights(&canSensorAccessories, "lhd", &CanSensorAccessories::getStatusHeadlights, 1);
LoggingCommand<CanSensorAccessories, int> urbanBrakelights(&canSensorAccessories, "lbk", &CanSensorAccessories::getStatusBrakelights, 1);
LoggingCommand<CanSensorAccessories, int> urbanHorn(&canSensorAccessories, "horn", &CanSensorAccessories::getStatusHorn, 1);
LoggingCommand<CanSensorAccessories, int> urbanHazards(&canSensorAccessories, "lhz", &CanSensorAccessories::getStatusHazards, 1);
LoggingCommand<CanSensorAccessories, int> urbanRightSig(&canSensorAccessories, "ltr", &CanSensorAccessories::getStatusRightSignal, 1);
LoggingCommand<CanSensorAccessories, int> urbanLeftSig(&canSensorAccessories, "ltl", &CanSensorAccessories::getStatusLeftSignal, 1);
LoggingCommand<CanSensorAccessories, int> urbanWipers(&canSensorAccessories, "wipe", &CanSensorAccessories::getStatusWipers, 5);

String publishName = "BQIngestion";

/**
 * @brief callback fn passed to gps which receieves current speed, which is sent as can message to steering 
 * 
 * @param speed current gps speed
 */
void speedCallbackGps(float speed) {
    if (speed <= 70.0f && speed >= 0.0f) {
		CanMessage message = CAN_MESSAGE_NULL;
    	message.id = CAN_TELEMETRY_GPS_DATA;
        message.data[0] = (uint8_t)(speed * 3.6f);
        message.dataLength = 1;
		canInterface.sendMessage(message);
    }
}

// Set Bms Remotely
int remoteSetBms(String command){
	DEBUG_SERIAL("#### REMOTE - Attempting to set BMS to " + command + "BMS module");

	if (command.equalsIgnoreCase("tiny")) {
		bmsManager.setBms(BmsManager::BmsOption::Tiny);
	} else if (command.equalsIgnoreCase("orion")) {
		bmsManager.setBms(BmsManager::BmsOption::Orion);
	} else {
		return -1;
	}

    return 1;
}

LoggingDispatcher* CurrentVehicle::buildLoggingDispatcher() {
    // added here because because this function is called on startup
    gps.setSpeedCallback(speedCallbackGps);
	
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
    DEBUG_SERIAL("Vertical Acceleration: " + gps.getIncline() + "° - ");
    DEBUG_SERIAL("Horizontal Accuracy: " + gps.getHorizontalAccuracy() + "m - ");
    DEBUG_SERIAL("Vertical Accuracy: " + gps.getVerticalAccuracy() + "m - ");
    DEBUG_SERIAL_LN("Satellites in View: " + String(gps.getSatellitesInView()));
    // Thermo
    DEBUG_SERIAL("Motor Temp: " + String(thermo1.getProbeTemp()) + "°C - ");
    DEBUG_SERIAL_LN("Motor Controller Temp: " + String(thermo2.getProbeTemp()) + "°C");
    // Steering
    DEBUG_SERIAL("Throttle: " + String(steering.getThrottle()) + "% - ");
    DEBUG_SERIAL("Ignition: " + BOOL_TO_STRING(steering.getIgnition()) + " - ");
    DEBUG_SERIAL("DMS: " + BOOL_TO_STRING(steering.getDms()) + " - ");
    DEBUG_SERIAL_LN("Brake: " + BOOL_TO_STRING(steering.getBrake()));
    // BMS
	DEBUG_SERIAL("Current Bms: " + bms->getHumanName() + " - ");
    DEBUG_SERIAL("Battery Voltage: " + bms->getBatteryVolt() + "v - ");
    DEBUG_SERIAL("Battery Current: " + bms->getBatteryCurrent() + "A - ");
    DEBUG_SERIAL("Max Cell Voltage: " + bms->getMaxVolt() + "v - ");
    DEBUG_SERIAL("Min Cell Voltage: " + bms->getMinVolt() + "v - ");
    DEBUG_SERIAL("Avg Cell Voltage: " + bms->getAvgVolt() + "v - ");
    DEBUG_SERIAL("State of Charge: " + bms->getSoc() + "% - ");
    DEBUG_SERIAL("BMS Status: " + bms->getStatusBmsString() + " - ");
    DEBUG_SERIAL("BMS Fault: " + BmsFault::toString(bms->getFault()) + " - ");
    DEBUG_SERIAL("BMS Temperature: " + String(bms->getTempBms()) + "°C - ");
    DEBUG_SERIAL("Min Battery Temperature: " + String(bms->getMaxBatteryTemp()) + "°C - ");
    DEBUG_SERIAL_LN("Max Battery Temperature: " + String(bms->getMinBatteryTemp()) + "°C");
    DEBUG_SERIAL_LN("Avg Battery Temperature: " + String(bms->getAvgBatteryTemp()) + "°C");

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
    bms->restart();
}

#endif