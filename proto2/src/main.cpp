#include "Particle.h"
#include "JsonMaker.h"
#include "Sensor_ECU.h"
#include "Sensor_GPS.h"
#include "Sensor_Thermo.h"

#define PUBLISH_INTERVAL_SECONDS 5

SYSTEM_THREAD(ENABLED);

JsonMaker json_maker;
Sensor_ECU ecu(&Serial1);
Sensor_GPS gps(1000);
Sensor_Thermo thermo_1(SPI, A5, 1000);

uint32_t last_publish = 0;


void setup() {
    Serial.begin(115200);
    pinMode(D7, OUTPUT);

    ecu.begin();
    gps.begin();

    Serial.println("Particle Connected!");
}

void loop() {

    // Check for full data frame from ECU in UART buffer
    ecu.handle();
    gps.handle();
    thermo_1.handle();

    // Publish a message on the interval
    if (millis() - last_publish >= PUBLISH_INTERVAL_SECONDS*1000){
        last_publish = millis();
        // Call makeJSON function
        json_maker.refresh();
        json_maker.add("PROTO-ECT", ecu.getECT());
        json_maker.add("PROTO-IAT", ecu.getIAT());
        json_maker.add("PROTO-RPM", ecu.getRPM());
        json_maker.add("PROTO-UBADC", ecu.getUbAdc());
        json_maker.add("PROTO-O2S", ecu.getO2S());
        json_maker.add("PROTO-SPARK", ecu.getSpark());
        // Particle.publish("Proto", json_maker.get(), PRIVATE, WITH_ACK);
        Serial.println("New JSON Message: " + json_maker.get());

        // Unsure how to output current position sentence, however all the components are there
        Serial.println("GPS Latitude: " + String(gps.getLatitude()) + " deg");
        Serial.println("GPS Longitude: " + String(gps.getLongitude()) + " deg");
        Serial.print("GPS Altitude: "); Serial.print(gps.getAltitude()); Serial.println("m above sea level");
        Serial.print("GPS Speed: "); Serial.print(gps.getSpeed()); Serial.println(" km/h");
        Serial.print("GPS Satellites in View: "); Serial.println(gps.getNumSatellites());
        Serial.print("Current Temperature (Thermo1): "); Serial.print(thermo_1.getTemp()); Serial.println("C");
    }



}



