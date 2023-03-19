#include "vehicle.h"

#ifdef FC

#include "USARTSerialWrapper.h"
#include "Lsm6dsoAccelerometerWrapper.h"
#include "SensorFcpControl.h"
#include "DriverDisplay.h"

USARTSerialWrapper serial(&Serial1);
Lsm6dsoAccelerometerWrapper lsm6(&SPI, A3);

// sensors
SensorGps gps(new SFE_UBLOX_GNSS());
SensorAccelerometer accel(&lsm6, ACCEL_NEGATIVE_Y, ACCEL_NEGATIVE_Z);
SensorThermo thermo1(&SPI, A5);
SensorThermo thermo2(&SPI, A4);
SensorSigStrength sigStrength;
SensorVoltage inVoltage;
SensorFcpControl cellStack(&serial);

// driver display
Adafruit_SH1107 ssh1107(64, 128);
DriverDisplay display(ssh1107);
TextElement<String> speedElement([]() { return gps.getHorizontalSpeed(); }, 3, String("spd "), 1);
TextElement<String> stackElement([]() { return cellStack.getStackVoltage(); }, 3, String("stk "), 1);

LoggingCommand<SensorSigStrength, int> signalStrength(&sigStrength, "sigstr", &SensorSigStrength::getStrength, 10);
LoggingCommand<SensorSigStrength, int> signalQuality(&sigStrength, "sigql", &SensorSigStrength::getQuality, 10);
LoggingCommand<SensorVoltage, String> voltage(&inVoltage, "vin", &SensorVoltage::getVoltage, 10);
LoggingCommand<SensorThermo, int> thermoInt(&thermo1, "tmpint", &SensorThermo::getInternalTemp, 5);

LoggingCommand<SensorGps, String> gpsLong(&gps, "lon", &SensorGps::getLongitude, 1);
LoggingCommand<SensorGps, String> gpsLat(&gps, "lat", &SensorGps::getLatitude, 1);
LoggingCommand<SensorGps, int> gpsHeading(&gps, "hea", &SensorGps::getHeading, 1);
LoggingCommand<SensorGps, String> gpsAltitude(&gps, "alt", &SensorGps::getAltitude, 1);
LoggingCommand<SensorGps, String> gpsHorSpeed(&gps, "hvel", &SensorGps::getHorizontalSpeed, 1);
LoggingCommand<SensorGps, String> gpsHorAccuracy(&gps, "haccu", &SensorGps::getHorizontalAccuracy, 10);
LoggingCommand<SensorGps, String> gpsVerAccuracy(&gps, "vaccu", &SensorGps::getVerticalAccuracy, 10);

LoggingCommand<SensorAccelerometer, String> accelerometerHorAccel(&accel, "hacce", &SensorAccelerometer::getHorizontalAcceleration, 1);
LoggingCommand<SensorAccelerometer, String> accelerometerVertAccel(&accel, "vacce", &SensorAccelerometer::getVerticalAcceleration, 1);
LoggingCommand<SensorAccelerometer, String> accelerometerIncline(&accel, "incl", &SensorAccelerometer::getIncline, 1);

LoggingCommand<SensorFcpControl, String> cellVoltage1(&cellStack, "cv1", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage2(&cellStack, "cv2", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage3(&cellStack, "cv3", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage4(&cellStack, "cv4", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage5(&cellStack, "cv5", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage6(&cellStack, "cv6", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage7(&cellStack, "cv7", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage8(&cellStack, "cv8", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage9(&cellStack, "cv9", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage10(&cellStack, "cv10", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage11(&cellStack, "cv11", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage12(&cellStack, "cv12", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage13(&cellStack, "cv13", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage14(&cellStack, "cv14", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage15(&cellStack, "cv15", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage16(&cellStack, "cv16", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage17(&cellStack, "cv17", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage18(&cellStack, "cv18", &SensorFcpControl::getNextCellVoltage, 1);
LoggingCommand<SensorFcpControl, String> cellVoltage19(&cellStack, "cv19", &SensorFcpControl::getNextCellVoltage, 1);

LoggingCommand<SensorFcpControl, String> cellStackVoltage(&cellStack, "stv", &SensorFcpControl::getStackVoltage, 1);

LoggingCommand<SensorThermo, int> thermoMotor(&thermo1, "tmpmot", &SensorThermo::getProbeTemp, 5);
LoggingCommand<SensorThermo, int> thermoFuelCell(&thermo2, "tmpfcs", &SensorThermo::getProbeTemp, 5);

String publishName = "BQIngestion";

// CurrentVehicle namespace definitions
LoggingDispatcher* CurrentVehicle::buildLoggingDispatcher() {
    LoggingDispatcherBuilder builder(&dataQ, publishName, IntervalCommand::getCommands());
    return builder.build();
}

void CurrentVehicle::setup() {
    speedElement.setPosition(2, 2);
    stackElement.setPosition(2, 38);
    speedElement.setMinTextLength(5); // 00.00
    stackElement.setMinTextLength(5); // 00.00
    display.addDisplayElement(&speedElement);
    display.addDisplayElement(&stackElement);
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
    DEBUG_SERIAL("Horizontal Acceleration: " + accel.getHorizontalAcceleration() + "m/s^2 - ");
    DEBUG_SERIAL("Vertical Acceleration: " + accel.getVerticalAcceleration() + "m/s^2 - ");
    DEBUG_SERIAL("Incline: " + accel.getIncline() + "rad - ");
    DEBUG_SERIAL("Horizontal Accuracy: " + gps.getHorizontalAccuracy() + "m - ");
    DEBUG_SERIAL("Vertical Accuracy: " + gps.getVerticalAccuracy() + "m - ");
    DEBUG_SERIAL_LN("Satellites in View: " + String(gps.getSatellitesInView()));
    // Thermo
    DEBUG_SERIAL("Motor Temp: " + String(thermo1.getProbeTemp()) + "°C - ");
    DEBUG_SERIAL_LN("Fuel Cell Temp: " + String(thermo2.getProbeTemp()) + "°C");
	// Fuel Cell voltages
	DEBUG_SERIAL_LN("Fuel Cell Voltages:");
	for (int i = 0; i < cellStack.getNumFuelCells(); i++) {
		if (i != 0 && i % (cellStack.getNumFuelCells() / 2) == 0) {
			DEBUG_SERIAL("\n");
		}
		DEBUG_SERIAL("Cell " + String(i + 1) + ": " + FLOAT_TO_STRING(cellStack.getCellVoltageByIndex(i), 2) + "V\t");
	}
	DEBUG_SERIAL_LN();
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