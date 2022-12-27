#include "vehicle.h"

// Sandbox vehicle for testing new sensors
#ifdef SANDBOX

#include "Lsm6dsoxAccelerometerWrapper.h"
#include "SensorAccelerometer.h"
#include "SensorThermo.h"

Adafruit_LSM6DSOX sox;
Lsm6dsoxAccelerometerWrapper lsm6(&sox, &SPI, A3);
SensorAccelerometer accel(&lsm6);
// SensorThermo thermo(&SPI, A4);

// CurrentVehicle namespace definitions
LoggingDispatcher* CurrentVehicle::buildLoggingDispatcher() {
    return nullptr;
}

void CurrentVehicle::debugSensorData() {
    // Diagnostic
    DEBUG_SERIAL_LN(" -- Accelerometer -- ");
    DEBUG_SERIAL_LN("Status: " + accel.getInitStatus());
    DEBUG_SERIAL_LN("Gyro:  < " + String(accel.getGyro().x) + ", " + String(accel.getGyro().y) + ", " + String(accel.getGyro().x) + " >");
    DEBUG_SERIAL_LN("Accel: < " + String(accel.getAccel().x) + ", "+ String(accel.getAccel().y) + ", " + String(accel.getAccel().z) + " >");
    DEBUG_SERIAL_LN("Total Accel: " + String(accel.getAccelMagnitude()));
    DEBUG_SERIAL_LN("Pitch: " + String(accel.getPitch()) + "rad");
    // DEBUG_SERIAL_LN(" -- Thermocouple -- ");
    // DEBUG_SERIAL_LN("Status: " + thermo.getInitStatus());
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