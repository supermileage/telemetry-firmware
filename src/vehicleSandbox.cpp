#include "vehicle.h"

// Sandbox vehicle for testing new sensors
#ifdef SANDBOX

#include "SensorAccelerometer.h"
#include "SensorThermo.h"

SensorAccelerometer accel(&SPI, A3);
// SensorThermo thermo(&SPI, A4);

// CurrentVehicle namespace definitions
LoggingDispatcher* CurrentVehicle::buildLoggingDispatcher() {
    return nullptr;
}

void CurrentVehicle::debugSensorData() {
    // Diagnostic
    DEBUG_SERIAL_LN(" -- Accelerometer -- ");
    DEBUG_SERIAL_LN("Status: " + accel.getInitStatus());
    DEBUG_SERIAL_LN("Gyro: < " + String(accel.getGyroX()) + ", " + String(accel.getGyroY()) + ", " + String(accel.getGyroZ()) + " >");
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