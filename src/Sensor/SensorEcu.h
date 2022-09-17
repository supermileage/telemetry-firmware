#ifndef _SENSOR_ECU_H_
#define _SENSOR_ECU_H_

#include "Sensor.h"
#include  "TelemetrySerial.h"

class SensorEcu : public Sensor {
    public:
		static const int32_t PacketSize;
		static const uint8_t Header1;
		static const uint8_t Header2;
		static const uint8_t Header3;
		static const uint8_t DataFieldLength;
		static const uint8_t ServiceId;
		
        /**
         * Constructor 
         * @param *serial bus receiving ECU data
         **/
        SensorEcu(TelemetrySerial *serial);

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
        int getRPM(bool &valid = Sensor::dummy);

        /**
        * @return Manifold Absolute Pressure, kPa
        * */
        String getMap(bool &valid = Sensor::dummy);

        /**
        * @return Throttle Position Sensor, %
        * */
        int getTPS(bool &valid = Sensor::dummy);

        /**
        * @return Engine Coolant Temperature, DegC
        * */
        int getECT(bool &valid = Sensor::dummy);

        /**
        * @return Intake Air Temperature, DegC
        * */
        int getIAT(bool &valid = Sensor::dummy);

        /**
        * @return Oxygen Sensor, V
        * */
        String getO2S(bool &valid = Sensor::dummy);

        /**
        * @return Spark (Advance/Retard), CrA
        * */
        int getSpark(bool &valid = Sensor::dummy);

        /**
        * @return Fuel Injector 1 PWM Duty Cycle, ms
        * */
        String getFuelPW1(bool &valid = Sensor::dummy);

        /**
        * @return Fuel Injector 2 PWM Duty Cycle, ms
        * */
        String getFuelPW2(bool &valid = Sensor::dummy);

        /**
        * @return Battery Voltage, V
        * */
        String getUbAdc(bool &valid = Sensor::dummy);

        /**
        * @return Whether or not the ECU is corrently on and connected to Telemetry
        * */
        int getOn(bool &valid = Sensor::dummy);

    private:
        TelemetrySerial* _serial;

        uint32_t _lastUpdate = 0;
        bool _valid = false;

        int _rpm = 0;
        float _map = 0;
        int _tps = 0;
        int _ect = 0;
        int _iat = 0;
        float _o2s = 0;
        int _spark = 0;
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
        float _interpretValue(uint8_t high, uint8_t low, float factor, float offset, bool isInt = false);
};

#endif
