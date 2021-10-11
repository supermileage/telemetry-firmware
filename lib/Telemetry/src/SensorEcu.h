#ifndef _SENSOR_ECU_H_
#define _SENSOR_ECU_H_

#include "Particle.h"
#include "Sensor.h"

class SensorEcu : public Sensor {
    public:
        /**
         * Constructor 
         * @param *serial bus receiving ECU data
         **/
        SensorEcu(USARTSerial *serial);

        /**
         * Begins ECU receive
         * */
        void begin();
        /**
         * Flushes all data out of Serial receive buffer
         * */
        void flush();

        /**
         * Checks Serial buffer for incoming data. If a full ECU data frame is in buffer, parses and saves it
         * */
        void handle();

        String getHumanName();

        /**
         * @return Timestamp of last ECU receive
         * */
        time_t getTimestamp();

        /**
         * @return Rotations Per Minute
         * */
        float getRPM();

        /**
        * @return Manifold Absolute Pressure
        * */
        float getMap();

        /**
        * @return Throttle Position Sensor
        * */
        float getTPS();

        /**
        * @return Engine Coolant Temperature
        * */
        float getECT();

        /**
        * @return Intake Air Temperature
        * */
        float getIAT();

        /**
        * @return Oxygen Sensor
        * */
        float getO2S();

        /**
        * @return Spark (Advance? Timing?)
        * */
        float getSpark();

        /**
        * @return Fuel Injector 1 PWM Duty Cycle
        * */
        float getFuelPW1();

        /**
        * @return Fuel Injector 2 PWM Duty Cycle
        * */
        float getFuelPW2();

        /**
        * @return Battery Voltage
        * */
        float getUbAdc();

    private:
        USARTSerial * _serial;
        time_t _time;
        float _rpm;
        float _map;
        float _tps;
        float _ect;
        float _iat;
        float _o2s;
        float _spark;
        float _fuelPW1;
        float _fuelPW2;
        float _ubAdc;

        /**
         * Helper for interpreting raw values from ECU
         * @param high byte
         * @param low byte
         * @param factor ??
         * @param offset ??
         * @return Intepreted value
         * */
        float _interpretValue(uint8_t high, uint8_t low, float factor, float offset);
};

#endif
