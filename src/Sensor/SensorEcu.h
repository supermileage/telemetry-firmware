#ifndef _SENSOR_ECU_H_
#define _SENSOR_ECU_H_

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
        void begin() override;
        
        /**
         * Flushes all data out of Serial receive buffer
         * */
        void flush();

        /**
         * Checks Serial buffer for incoming data. If a full ECU data frame is in buffer, parses and saves it
         * */
        void handle() override;

        String getHumanName() override;

        /**
         * @return Engine Speed, RPM
         * */
        String getRPM();

        /**
        * @return Manifold Absolute Pressure, kPa
        * */
        String getMap();

        /**
        * @return Throttle Position Sensor, %
        * */
        String getTPS();

        /**
        * @return Engine Coolant Temperature, DegC
        * */
        String getECT();

        /**
        * @return Intake Air Temperature, DegC
        * */
        String getIAT();

        /**
        * @return Oxygen Sensor, V
        * */
        String getO2S();

        /**
        * @return Spark (Advance/Retard), CrA
        * */
        String getSpark();

        /**
        * @return Fuel Injector 1 PWM Duty Cycle, ms
        * */
        String getFuelPW1();

        /**
        * @return Fuel Injector 2 PWM Duty Cycle, ms
        * */
        String getFuelPW2();

        /**
        * @return Battery Voltage, V
        * */
        String getUbAdc();

    private:
        USARTSerial * _serial;
        float _rpm = 0;
        float _map = 0;
        float _tps = 0;
        float _ect = 0;
        float _iat = 0;
        float _o2s = 0;
        float _spark = 0;
        float _fuelPW1 = 0;
        float _fuelPW2 = 0;
        float _ubAdc = 0;

        /**
         * Helper for interpreting raw values from ECU
         * @param high byte
         * @param low byte
         * @param factor 
         * @param offset 
         * @return Intepreted value
         * */
        float _interpretValue(uint8_t high, uint8_t low, float factor, float offset);
};

#endif
