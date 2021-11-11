#define PROTO 0
#define URBAN 1
#define FC 2

#define CURRENT_VEHICLE 2

#if (CURRENT_VEHICLE == PROTO)
    #include "proto_globals.h"
#elif (CURRENT_VEHICLE == URBAN)
    #include "urban_globals.h"
#else
    #include "fc_globals.h"
#endif
#include "Sensor.h"
#include "SensorGps.h"
#include "SensorThermo.h"
#include "SensorSigStrength.h"
#include "SensorVoltage.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

SensorGps gps(new SFE_UBLOX_GNSS(), GPS_UPDATE_FREQUENCY);
SensorThermo thermo1(&SPI, A5);
SensorThermo thermo2(&SPI, A4);
SensorSigStrength sigStrength;
SensorVoltage inVoltage;

Sensor *sensors[] = {&gps, &thermo1, &thermo2, &sigStrength, &inVoltage};

Led led_orange(A0, 63);
// Blue LED to flash on startup, go solid when valid time has been established
Led led_blue(D7, 255);
Led led_green(D8, 40);

DataQueue dataQ(VEHICLE_NAME);
Dispatcher *dispatcher;
unsigned long lastPublish = 0;

void publishMessage() {
    long start, json_build_time;
    if (DEBUG_CPU_TIME) {
        start = micros();
    }

    dataQ.add("PropertyName", "Value");

    if (DEBUG_CPU_TIME) {
        json_build_time = micros() - start;
    }

    DEBUG_SERIAL_LN("------------------------");
    DEBUG_SERIAL_LN("Time: " + Time.timeStr());
    if(PUBLISH_ENABLED){
        DEBUG_SERIAL_LN("FC - Publish ENABLED - Message: ");
        // Publish to Particle Cloud
        DEBUG_SERIAL_LN(dataQ.publish("Proto", PRIVATE, WITH_ACK));
    }else{
        DEBUG_SERIAL_LN("FC - Publish DISABLED - Message: ");
        DEBUG_SERIAL_LN(dataQ.resetData());
    }

    CurrentVehicle::publishMessage();

    if(DEBUG_MEM) {
        DEBUG_SERIAL("\nFREE RAM: " + String(System.freeMemory()) + "B / 128000B");

    if(DEBUG_SENSOR_ENABLE){
        DEBUG_SERIAL_LN("");
        DEBUG_SERIAL_LN("SENSOR READINGS: ");
        // Diagnostic
        DEBUG_SERIAL("Signal Strength: " + sigStrength.getStrength() + " % - ");
        DEBUG_SERIAL("Signal Quality: " + sigStrength.getQuality() + " % - ");
        DEBUG_SERIAL_LN("Input Voltage: "+ String(inVoltage.getVoltage()) + " V");
        // Thermocouples
        DEBUG_SERIAL("Temperature (Thermo1): " + thermo1.getProbeTemp() + "°C - ");
        DEBUG_SERIAL("Temperature (Thermo2): " + thermo2.getProbeTemp() + "°C - ");
        DEBUG_SERIAL_LN("Internal Temperature (Thermo1): " + thermo1.getInternalTemp() + "°C");
        // GPS
        DEBUG_SERIAL("Longitude: " + gps.getLongitude() + "° - ");
        DEBUG_SERIAL("Latitude: " + gps.getLatitude() + "° - ");
        DEBUG_SERIAL("Horizontal Acceleration: " + gps.getHorizontalAcceleration() + " m/s^2 - ");
        DEBUG_SERIAL("Altitude: " + gps.getAltitude() + " m - ");
        DEBUG_SERIAL("Vertical Acceleration: " + gps.getHorizontalAcceleration() + " m/s^2 - ");
        DEBUG_SERIAL("Horizontal Accuracy: " + gps.getHorizontalAccuracy() + " m - ");
        DEBUG_SERIAL("Vertical Accuracy: " + gps.getVerticalAccuracy() + " m - ");  
        DEBUG_SERIAL_LN("Satellites in View: " + gps.getSatellitesInView());
        DEBUG_SERIAL_LN();
    }
    
    if(DEBUG_MEM){
        DEBUG_SERIAL_LN("\nFREE RAM: " + String(System.freeMemory()) + "B / 128000B");
    }

    // Output CPU time in microseconds spent on each task
    if (DEBUG_CPU_TIME) {
        DEBUG_SERIAL("\nCPU Time:");
        DEBUG_SERIAL("Build JSON Message: " + String(json_build_time) + "us");
        for (unsigned i = 0; i < sensor_count; i++) {
            DEBUG_SERIAL(sensors[i]->getHumanName() + " polling: " + String(sensors[i]->getLongestHandleTime()) + "us");
        }
        DEBUG_SERIAL_LN();
    }
}

/**
 * 
 * SETUP
 * 
 * */
void setup() {
    if(DEBUG_SERIAL_ENABLE){
        Serial.begin(115200);
    }

    // Start i2c with clock speed of 400 KHz
    // This requires the pull-up resistors to be removed on i2c bus
    Wire.setClock(400000);
    Wire.begin();

    Time.zone(TIME_ZONE);

    for (unsigned int i = 0; i < sensor_count; i++) {
        sensors[i]->begin();
    }

    DispatcherBuilder builder(commands, command_count, &dataQ);
    dispatcher = builder.build();

    led_blue.flashRepeat(500);

    CurrentVehicle::setupMessage();
}

/**
 * 
 * LOOP
 * 
 * */
void loop() {
    // Sensor Handlers
    for (unsigned int i = 0; i < sensor_count; i++) {
        if (DEBUG_CPU_TIME) {
            sensors[i]->benchmarkedHandle();
        } else {
            sensors[i]->handle();
        }
    }

    dataQ.loop();
    dispatcher->run();

    // LED Handlers
    led_orange.handle();
    led_blue.handle();
    led_green.handle();

    // Publish a message every publish interval
    if (millis() - lastPublish >= PUBLISH_INTERVAL_MS){
        // If no valid time pulled from cellular, attempt to get valid time from GPS (should be faster)
        if(!Time.isValid()){
            if(gps.getTimeValid()){
                led_blue.on();
                Time.setTime(gps.getUnixTime());
            }
        }else{
            led_blue.on();
        }

        lastPublish = millis();
        publishMessage();
    }

    
}