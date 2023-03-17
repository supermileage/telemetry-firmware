#include "vehicle.h"

// Sandbox vehicle for testing new sensors
#ifdef SANDBOX

#define TEST_ACCELEROMETER
// #define TEST_DRIVER_DISPLAY

#include "Lsm6dsoAccelerometerWrapper.h"
#include "SensorAccelerometer.h"
#include "SensorThermo.h"
#include "DriverDisplay.h"

#ifdef TEST_ACCELEROMETER
Lsm6dsoAccelerometerWrapper lsm6(&SPI, A3);
SensorAccelerometer accel(&lsm6);
#endif

#ifdef TEST_DRIVER_DISPLAY
SensorGps gps(new SFE_UBLOX_GNSS());
Adafruit_SH1107 ssh1107(64, 128);
DriverDisplay display(ssh1107);
TextElement<String> speedElement1([]() { return gps.getHorizontalSpeed(); }, 3, String("  spd: "), 1);
TextElement<String> speedElement2([]() { return gps.getHorizontalSpeed(); }, 3, String("  rpm: "), 1);
#endif

// CurrentVehicle namespace definitions
LoggingDispatcher* CurrentVehicle::buildLoggingDispatcher() {
    return nullptr;
}

void CurrentVehicle::setup() {
    #ifdef TEST_DRIVER_DISPLAY
	speedElement1.setPosition(4, 2);
    speedElement2.setPosition(4, 38);
    display.addDisplayElement(&speedElement1);
    display.addDisplayElement(&speedElement2);
	#endif
}

void CurrentVehicle::debugSensorData() {
    // Diagnostic
	#ifdef TEST_ACCELEROMETER
    DEBUG_SERIAL_LN(" -- Accelerometer -- ");
    DEBUG_SERIAL_LN("Status: " + accel.getInitStatus());
    DEBUG_SERIAL_LN("Gyro:  < " + String(accel.getGyro().x) + ", " + String(accel.getGyro().y) + ", " + String(accel.getGyro().x) + " >");
    DEBUG_SERIAL_LN("Accel: < " + String(accel.getAccel().x) + ", "+ String(accel.getAccel().y) + ", " + String(accel.getAccel().z) + " >");
    DEBUG_SERIAL_LN("Horizontal Accel: " + accel.getHorizontalAcceleration());
    DEBUG_SERIAL_LN("Verical Accel: " + accel.getVerticalAcceleration());
    DEBUG_SERIAL_LN("Pitch: " + accel.getIncline() + "rad");
	#endif

	#ifdef TEST_DRIVER_DISPLAY
	DEBUG_SERIAL_LN("Gps Init Status: " + gps.getInitStatus());
	DEBUG_SERIAL_LN("Display Status: " + display.getInitStatus());
    DEBUG_SERIAL_LN("");
    DEBUG_SERIAL("Longitude: " + gps.getLongitude() + "° - ");
    DEBUG_SERIAL("Latitude: " + gps.getLatitude() + "° - ");
    DEBUG_SERIAL("Heading: " + String(gps.getHeading()) + "° - ");
    DEBUG_SERIAL("Altitude: " + gps.getAltitude() + "m - ");
    DEBUG_SERIAL("Horizontal Accuracy: " + gps.getHorizontalAccuracy() + "m - ");
    DEBUG_SERIAL("Vertical Accuracy: " + gps.getVerticalAccuracy() + "m - ");
    DEBUG_SERIAL_LN("Satellites in View: " + String(gps.getSatellitesInView()));
	#endif
}

bool CurrentVehicle::getTimeValid() {
    return false;
}

uint32_t CurrentVehicle::getUnixTime() {
    return 0;
}

void CurrentVehicle::toggleGpsOverride() {
    
}

void CurrentVehicle::restartTinyBms() {

}
 
#endif