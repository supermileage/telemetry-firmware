#include "vehicle.h"

#ifdef PROTO


#include "Lsm6dsoAccelerometerWrapper.h"
#include "USARTSerialWrapper.h"
#include "SensorEcu.h"
#include "DriverDisplay.h"

#include "Mcp2515CanWrapper.h"
#include "CanInterface.h"
#include "CanListener.h"
#include "CanSensorSpeeduinoECU.h"


// forward declarations

//Change: MAX_RPM
const int MAX_RPM = 3000;
String computeHorizontalSpeed();

//new functions
String computeAccelerationMagnitude();
String computeRpmWarning();

Mcp2515CanWrapper canBus(&SPI1, D5, D6);
CanInterface canInterface((CanController*)&canBus);

USARTSerialWrapper usartSerial(&Serial1);
Lsm6dsoAccelerometerWrapper lsm6(&SPI, A3);

// sensors
CanSensorSpeeduinoECU speeduinoECU(canInterface);
CanSensorSpeeduinoECU* speedecu = &speeduinoECU;

SensorEcu ecu(&usartSerial);
SensorGps gps(new SFE_UBLOX_GNSS());
SensorAccelerometer accel(&lsm6, ACCEL_POSITIVE_Z, ACCEL_POSITIVE_Y);
SensorThermo thermo1(&SPI, A5);
SensorThermo thermo2(&SPI, A4);
SensorSigStrength sigStrength;
SensorVoltage inVoltage;

// driver display
Adafruit_SH1107 ssh1107(64, 128);
DriverDisplay display(ssh1107);
// TextElement constructor params: displayFunc, textSize, textColour, labelSize, labelString
TextElement<String> speedElement(&computeHorizontalSpeed, 2, SH110X_WHITE, 1, String("spd "));
TextElement<int> rpmElement([]() { return ecu.getOn() ? ecu.getRPM() : 0; }, 2, 1, SH110X_WHITE, String("rpm "));

// Changes: accelElement and  warningElement
TextElement<String> accelElement(&computeAccelerationMagnitude, 2, SH110X_WHITE, 1, String("acc "));
TextElement<String> warningElement(&computeRpmWarning, 4, SH110X_WHITE, 1, String(""));

// commands
LoggingCommand<SensorSigStrength, int> signalStrength(&sigStrength, "sigstr", &SensorSigStrength::getStrength, 10);
LoggingCommand<SensorSigStrength, int> signalQuality(&sigStrength, "sigql", &SensorSigStrength::getQuality, 10);
LoggingCommand<SensorVoltage, String> voltage(&inVoltage, "vin", &SensorVoltage::getVoltage, 10);
LoggingCommand<SensorThermo, int> thermoInt(&thermo1, "tmpint", &SensorThermo::getInternalTemp, 5);

LoggingCommand<SensorGps, String> gpsLong(&gps, "lon", &SensorGps::getLongitude, 1);
LoggingCommand<SensorGps, String> gpsLat(&gps, "lat", &SensorGps::getLatitude, 1);
LoggingCommand<SensorGps, int> gpsHeading(&gps, "hea", &SensorGps::getHeading, 1);
LoggingCommand<SensorGps, String> gpsAltitude(&gps, "alt", &SensorGps::getAltitude, 1);
LoggingCommand<SensorGps, String> gpsHorSpeed(&gps, "hvel", &SensorGps::getHorizontalSpeed, 1);
// LoggingCommand<SensorGps, String> gpsHorAccel(&gps, "hacce", &SensorGps::getHorizontalAcceleration, 1);
// LoggingCommand<SensorGps, String> gpsVertAccel(&gps, "vacce", &SensorGps::getVerticalAcceleration, 1);
// LoggingCommand<SensorGps, String> gpsIncline(&gps, "incl", &SensorGps::getIncline, 1);
LoggingCommand<SensorGps, String> gpsHorAccuracy(&gps, "haccu", &SensorGps::getHorizontalAccuracy, 10);
LoggingCommand<SensorGps, String> gpsVerAccuracy(&gps, "vaccu", &SensorGps::getVerticalAccuracy, 10);

LoggingCommand<SensorAccelerometer, String> accelerometerHorAccel(&accel, "hacce", &SensorAccelerometer::getHorizontalAcceleration, 1);
LoggingCommand<SensorAccelerometer, String> accelerometerVertAccel(&accel, "vacce", &SensorAccelerometer::getVerticalAcceleration, 1);
LoggingCommand<SensorAccelerometer, String> accelerometerIncline(&accel, "incl", &SensorAccelerometer::getIncline, 1);

LoggingCommand<SensorThermo, int> thermoHead(&thermo1, "tmphead", &SensorThermo::getProbeTemp, 5);
LoggingCommand<SensorThermo, int> thermoCrank(&thermo2, "tmpcnk", &SensorThermo::getProbeTemp, 5);
/*
LoggingCommand<SensorEcu, int> ecuOn(&ecu, "eon", &SensorEcu::getOn, 1);
LoggingCommand<SensorEcu, int> ecuRpm(&ecu, "rpm", &SensorEcu::getRPM, 1);
LoggingCommand<SensorEcu, String> ecuMap(&ecu, "map", &SensorEcu::getMap, 1);
LoggingCommand<SensorEcu, int> ecuTps(&ecu, "tps", &SensorEcu::getTPS, 1);
LoggingCommand<SensorEcu, int> ecuEct(&ecu, "tmpblk", &SensorEcu::getECT, 5);
LoggingCommand<SensorEcu, int> ecuIat(&ecu, "tmpia", &SensorEcu::getIAT, 5);
LoggingCommand<SensorEcu, String> ecuO2s(&ecu, "o2s", &SensorEcu::getO2S, 1);
LoggingCommand<SensorEcu, int> ecuSpark(&ecu, "spar", &SensorEcu::getSpark, 1);
LoggingCommand<SensorEcu, String> ecuFuel(&ecu, "pw1", &SensorEcu::getFuelPW1, 1);
*/

//Speedino ECU
LoggingCommand<CanSensorSpeeduinoECU, String> ecuSecl(speedecu, "SECL", &CanSensorSpeeduinoECUtemp::getcurrentStatus_secl, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuSquirt(speedecu, "Squirt", &CanSensorSpeeduinoECUtemp::getcurrentStatus_squirt, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuEngine(speedecu, "Engine", &CanSensorSpeeduinoECUtemp::getcurrentStatus_engine, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuDwell(speedecu, "Dwell", &CanSensorSpeeduinoECUtemp::getcurrentStatus_dwell, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuMAP(speedecu, "MAP", &CanSensorSpeeduinoECUtemp::getcurrentStatus_MAP, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuIAT(speedecu, "IAT", &CanSensorSpeeduinoECUtemp::getcurrentStatus_IAT, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuCoolant(speedecu, "Coolant", &CanSensorSpeeduinoECUtemp::getcurrentStatus_coolant, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuTpsADC(speedecu, "TPS ADC", &CanSensorSpeeduinoECUtemp::getcurrentStatus_tpsADC, 5);

// ================= CAN 3101 =================
LoggingCommand<CanSensorSpeeduinoECU, String> ecuBattery10(speedecu, "Battery", &CanSensorSpeeduinoECUtemp::getcurrentStatus_battery10, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuO2(speedecu, "O2", &CanSensorSpeeduinoECUtemp::getcurrentStatus_O2, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuEgoCorrection(speedecu, "Ego Correction", &CanSensorSpeeduinoECUtemp::getcurrentStatus_egoCorrection, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuIatCorrection(speedecu, "IAT Correction", &CanSensorSpeeduinoECUtemp::getcurrentStatus_iatCorrection, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuWueCorrection(speedecu, "WUE Correction", &CanSensorSpeeduinoECUtemp::getcurrentStatus_wueCorrection, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuRPM(speedecu, "RPM", &CanSensorSpeeduinoECUtemp::getcurrentStatus_RPM, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuTAEamount(speedecu, "TAE Amount", &CanSensorSpeeduinoECUtemp::getcurrentStatus_TAEamount, 5);

// ================= CAN 3102 =================
LoggingCommand<CanSensorSpeeduinoECU, String> ecuBarometerCorrection(speedecu, "Barometer Correction", &CanSensorSpeeduinoECUtemp::getcurrentStatus_barometerCorrection, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuCorrections(speedecu, "Total GammaE", &CanSensorSpeeduinoECUtemp::getcurrentStatus_corrections, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuVE(speedecu, "CurrentVE", &CanSensorSpeeduinoECUtemp::getcurrentStatus_VE, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuAfrTarget(speedecu, "AFR Target", &CanSensorSpeeduinoECUtemp::getcurrentStatus_afrTarget, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuPW(speedecu, "Pulse Width", &CanSensorSpeeduinoECUtemp::getcurrentStatus_PW, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuTpsDOT(speedecu, "TPS DOT", &CanSensorSpeeduinoECUtemp::getcurrentStatus_tpsDOT, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuAdvance(speedecu, "Advance", &CanSensorSpeeduinoECUtemp::getcurrentStatus_advance, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuTPS(speedecu, "TPS", &CanSensorSpeeduinoECUtemp::getcurrentStatus_TPS, 5);

// ================= CAN 3103 =================
LoggingCommand<CanSensorSpeeduinoECU, String> ecuLoopsPerSecond(speedecu, "Loops Per Second", &CanSensorSpeeduinoECUtemp::getcurrentStatus_loopsPerSecond, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuFreeRAM(speedecu, "Free RAM", &CanSensorSpeeduinoECUtemp::getcurrentStatus_freeRAM, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuBatCorrection(speedecu, "Battery Correction", &CanSensorSpeeduinoECUtemp::getcurrentStatus_batCorrection, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuSpark(speedecu, "Spark", &CanSensorSpeeduinoECUtemp::getcurrentStatus_spark, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuO2_2(speedecu, "O2_2", &CanSensorSpeeduinoECUtemp::getcurrentStatus_O2_2, 5);

// ================= CAN 3104 =================
LoggingCommand<CanSensorSpeeduinoECU, String> ecuRpmDOT(speedecu, "RPM DOT", &CanSensorSpeeduinoECUtemp::getcurrentStatus_rpmDOT, 5);
LoggingCommand<CanSensorSpeeduinoECU, String> ecuFlex(speedecu, "Flex Sensor", &CanSensorSpeeduinoECUtemp::getcurrentStatus_flex, 5);


String publishName = "BQIngestion";

// CurrrentVehicle namespace definitions
LoggingDispatcher* CurrentVehicle::buildLoggingDispatcher() {
    LoggingDispatcherBuilder builder(&dataQ, publishName, IntervalCommand::getCommands());
    return builder.build();
}

void CurrentVehicle::setup() {
    speedElement.setPosition(2, 2);
    rpmElement.setPosition(2, 22);
    speedElement.setMinTextLength(5); // 00.00
    rpmElement.setMinTextLength(4);   // 00000
    display.addDisplayElement(&speedElement);
    display.addDisplayElement(&rpmElement);
    
    //changes: accelElement
    accelElement.setPosition(2, 42);
    accelElement.setMinTextLength(5); // 00.00
    display.addDisplayElement(&accelElement);
    
    //rpm limit warning
    warningElement.setPosition(100, 10);
    warningElement.setMinTextLength(1); // *
    display.addDisplayElement(&warningElement);          
}

void CurrentVehicle::debugSensorData() {
    // System
    DEBUG_SERIAL("Signal Strength: " + String(sigStrength.getStrength()) + "% - ");
    DEBUG_SERIAL("Signal Quality: " + String(sigStrength.getQuality()) + "% - ");
    DEBUG_SERIAL("Input Voltage: "+ String(inVoltage.getVoltage()) + "v - ");
    DEBUG_SERIAL_LN("Internal Temperature (Thermo1): " + String(thermo1.getInternalTemp()) + "°C");
    // GPS
	DEBUG_SERIAL("GPS Init: " + gps.getInitStatus() + " - ");
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
    DEBUG_SERIAL_LN("Engine Head Temp: " + String(thermo1.getProbeTemp()) + "°C");
    DEBUG_SERIAL_LN("Engine Crankcase Temp: " + String(thermo2.getProbeTemp()) + "°C");
    /*
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
    */

    //Speeduino ECU
    // ================= CAN 3100 =================
    DEBUG_SERIAL("SECL: " + speedecu->getcurrentStatus_secl() + " - ");
    DEBUG_SERIAL("Squirt: " + speedecu->getcurrentStatus_squirt() + " - ");
    DEBUG_SERIAL("Engine: " + speedecu->getcurrentStatus_engine() + " - ");
    DEBUG_SERIAL_LN("Dwell: " + speedecu->getcurrentStatus_dwell() + "ms");
    DEBUG_SERIAL("MAP: " + speedecu->getcurrentStatus_MAP() + " - ");
    DEBUG_SERIAL("IAT: " + speedecu->getcurrentStatus_IAT() + "°C - ");
    DEBUG_SERIAL("Coolant: " + speedecu->getcurrentStatus_coolant() + "°C - ");
    DEBUG_SERIAL_LN("TPS ADC: " + speedecu->getcurrentStatus_tpsADC());

    // ================= CAN 3101 =================
    DEBUG_SERIAL("Battery: " + speedecu->getcurrentStatus_battery10() + "V - ");
    DEBUG_SERIAL("O2: " + speedecu->getcurrentStatus_O2() + " - ");
    DEBUG_SERIAL("Ego Correction: " + speedecu->getcurrentStatus_egoCorrection() + "% - ");
    DEBUG_SERIAL_LN("IAT Correction: " + speedecu->getcurrentStatus_iatCorrection() + "%");
    DEBUG_SERIAL("WUE Correction: " + speedecu->getcurrentStatus_wueCorrection() + "% - ");
    DEBUG_SERIAL("RPM: " + speedecu->getcurrentStatus_RPM() + " - ");
    DEBUG_SERIAL_LN("TAE Amount: " + speedecu->getcurrentStatus_TAEamount() + "%");

    // ================= CAN 3102 =================
    DEBUG_SERIAL("Barometer Correction: " + speedecu->getcurrentStatus_barometerCorrection() + "% - ");
    DEBUG_SERIAL("Total Gamma E: " + speedecu->getcurrentStatus_corrections() + "% - ");
    DEBUG_SERIAL_LN("Current VE: " + speedecu->getcurrentStatus_VE() + "%");
    DEBUG_SERIAL("AFR Target: " + speedecu->getcurrentStatus_afrTarget() + " - ");
    DEBUG_SERIAL("Pulse Width: " + speedecu->getcurrentStatus_PW() + "ms - ");
    DEBUG_SERIAL("TPS DOT: " + speedecu->getcurrentStatus_tpsDOT() + " - ");
    DEBUG_SERIAL_LN("Advance: " + speedecu->getcurrentStatus_advance() + "°");
    DEBUG_SERIAL_LN("TPS: " + speedecu->getcurrentStatus_TPS() + "%");

    // ================= CAN 3103 =================
    DEBUG_SERIAL("Loops Per Second: " + speedecu->getcurrentStatus_loopsPerSecond() + " - ");
    DEBUG_SERIAL_LN("Free RAM: " + speedecu->getcurrentStatus_freeRAM() + " bytes");
    DEBUG_SERIAL("Battery Correction: " + speedecu->getcurrentStatus_batCorrection() + "% - ");
    DEBUG_SERIAL("Spark: " + speedecu->getcurrentStatus_spark() + " - ");
    DEBUG_SERIAL_LN("O2_2: " + speedecu->getcurrentStatus_O2_2());

    // ================= CAN 3104 =================
    DEBUG_SERIAL("RPM DOT: " + speedecu->getcurrentStatus_rpmDOT() + " - ");
    DEBUG_SERIAL_LN("Flex Sensor: " + speedecu->getcurrentStatus_flex() + "%");

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

String computeHorizontalSpeed() {
    if (ecu.getOn())
        // 9.7 : 1 gear ratio -- 21" diameter wheels -- 39370.1 inches per kilometer
        return FLOAT_TO_STRING((float)ecu.getRPM() / 9.7 * 3.14 * 21 * 60 / 39370.1, 2);
    else
        return gps.getHorizontalSpeed();
}

//Changes: computeAccelerationMagnitude() andc computeRpmWarning()
String computeAccelerationMagnitude() {
    if (accel.getInitStatus() == "Success") 
        return FLOAT_TO_STRING(accel.getHorizontalAcceleration(), 5); 
    else
        return gps.getHorizontalAcceleration();
}

String computeRpmWarning() {
    if ([]() { return ecu.getOn() ? ecu.getRPM() : 0; }() >= MAX_RPM) 
        return String("*"); //warning activated
    
    else 
        return String(" "); //no warning
}

#endif