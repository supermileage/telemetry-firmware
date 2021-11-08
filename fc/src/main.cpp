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

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);

Led led_orange(A0, 63);
// Blue LED to flash on startup, go solid when valid time has been established
Led led_blue(D7, 255);
Led led_green(D8, 40);

DataQueue dataQ(VEHICLE_NAME);
Dispatcher *dispatcher;
unsigned long lastPublish = 0;

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

    for (Sensor *s : sensors) {
        s->begin();
    }

    DispatcherBuilder builder(commands, &dataQ);
    dispatcher = builder.build();

    led_blue.flashRepeat(500);
}

/**
 * 
 * LOOP
 * 
 * */
void loop() {
    // Sensor Handlers
    for (Sensor *s : sensors) {
        if (DEBUG_CPU_TIME) {
            s->benchmarkedHandle();
        } else {
            s->handle();
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
        SerialDebugPublishing::publishMessage();
    }
}