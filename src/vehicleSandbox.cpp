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
Adafruit_SH1107 display(64, 128);
bool g_displayInitialized = false;
#endif

// CurrentVehicle namespace definitions
LoggingDispatcher* CurrentVehicle::buildLoggingDispatcher() {
	#ifdef TEST_I2C
	g_displayInitialized = display.begin(0x3C, false);
	if (g_displayInitialized) {
        display.setRotation(1);
		display.clearDisplay();
	}
	#endif
    return nullptr;
}

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

void CurrentVehicle::loop() {
    display.clearDisplay();

    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    
    display.cp437(true);

    const char hello[] = "Hello";
    const char world[] = "World!";

    display.setCursor(32, 16);
    for(int16_t i=0; i<6; i++) {
        display.write((uint8_t)hello[i]);
    }

    display.setCursor(28, 36);
    for(int16_t i=0; i<7; i++) {
        display.write((uint8_t)world[i]);
    }

    display.display();
    delay(2000);
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
	DEBUG_SERIAL_LN("SH1107 Status: " + (String)(g_displayInitialized ? "Success" : "Failed"));
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