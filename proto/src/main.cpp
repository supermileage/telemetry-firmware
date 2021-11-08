#define PROTO 0
#define URBAN 1
#define FC 2

#define CURRENT_VEHICLE 0

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
uint32_t lastPublish = 0;


/**
 * 
 * SETUP
 * 
 * */
void setup() {
    if(DEBUG_SERIAL_ENABLE){
        Serial.begin(115200);
    }

    Time.zone(TIME_ZONE);

    for (unsigned i = 0; i < sensor_count; i++) {
        sensors[i]->begin();
    }

    DispatcherBuilder builder(commands, command_count, &dataQ);
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
    for (unsigned i = 0; i < sensor_count; i++) {
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
        SerialDebugPublishing::publishMessage();
    }
}



