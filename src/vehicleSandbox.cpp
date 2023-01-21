#include "vehicle.h"

// Sandbox vehicle for testing new sensors
#ifdef SANDBOX

// #define TEST_ACCELEROMETER
#define TEST_I2C

#include "Adafruit_SH110X.h"
#include "Lsm6dsoAccelerometerWrapper.h"
#include "SensorAccelerometer.h"
#include "SensorThermo.h"

#ifdef TEST_ACCELEROMETER
Lsm6dsoAccelerometerWrapper lsm6(&SPI, A3);
SensorAccelerometer accel(&lsm6);
#endif

#ifdef TEST_I2C
SensorGps gps(new SFE_UBLOX_GNSS());
Adafruit_SH1107 sh1107(20, 20);
#endif

// CurrentVehicle namespace definitions
LoggingDispatcher* CurrentVehicle::buildLoggingDispatcher() {
    return nullptr;
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
	#ifdef TEST_I2C
	DEBUG_SERIAL_LN("Gps Init Status: " + gps.getInitStatus());
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