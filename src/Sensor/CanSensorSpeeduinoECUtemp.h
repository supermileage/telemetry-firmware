#ifndef _CAN_SENSOR_SPEEDUINO_ECU_TEMP_H_
#define _CAN_SENSOR_SPEEDUINO_ECU_TEMP_H_

#include "CanListener.h"
#include "CanInterface.h"

//only necessary if telemetry sends CAN messages
#define SOC_UPDATE_INTERVAL 495 // slighty off to avoid overlapping with other can write intervals

class CanSensorSpeeduinoECUtemp : public CanListener {
    public:
        CanSensorSpeeduinoECUtemp(CanInterface& caninterface);

        CanSensorSpeeduinoECUtemp(CanInterface& caninterface, uint16_t id);

        ~CanSensorSpeeduinoECUtemp();

        void handle() override;

        uint64_t getLastUpdateTime();

        void setIsAsleep(bool value);

		bool getIsAsleep();

        //in case of of states pased bytes
        // enum ECUStatus { Charging, Charged, Discharging, Regeneration, Idle, FaultError, ChargeEnabled, DischargeEnabled, Unknown };  
        /*   
                byte 1 - currentStatus.squirt; //Squirt Bitfield
                byte 2 - currentStatus.engine; //Engine Status Bitfield
                byte 3 - currentStatus.dwell; //Dwell in ms * 10
                byte 4 - currentStatus.MAP >> 1; //map value is divided by 2
                byte 5 - currentStatus.IAT + CALIBRATION_TEMPERATURE_OFFSET; //mat
                byte 6 - currentStatus.coolant + CALIBRATION_TEMPERATURE_OFFSET; //Coolant ADC
                byte 7 - currentStatus.tpsADC; //TPS (Raw 0-255)
                Can address 3101:

                byte 0 - currentStatus.battery10; //battery voltage
                byte 1 - currentStatus.O2; //O2
                byte 2 - currentStatus.egoCorrection; //Exhaust gas correction (%)
                byte 3 - currentStatus.iatCorrection; //Air temperature Correction (%)
                byte 4 - currentStatus.wueCorrection; //Warmup enrichment (%)
                byte 5 - lowByte(currentStatus.RPM); //rpm LB
                byte 6 - highByte(currentStatus.RPM); //rpm HB
                byte 7 - currentStatus.TAEamount; //acceleration enrichment (%)
                Can address 3102:

                byte 0 - 0x00; //Barometer correction (%)
                byte 1 - currentStatus.corrections; //Total GammaE (%)
                byte 2 - currentStatus.VE; //Current VE 1 (%)
                byte 3 - currentStatus.afrTarget;
                byte 4 - currentStatus.PW / 100; //Pulsewidth 1 multiplied by 10 in ms. Have to convert from uS to mS.
                byte 5 - currentStatus.tpsDOT; //TPS DOT
                byte 6 - currentStatus.advance;
                byte 7 - currentStatus.TPS; // TPS (0% to 100%)
                Can address 3103:

                byte 0 - lowByte(currentStatus.loopsPerSecond);
                byte 1 - highByte(currentStatus.loopsPerSecond);
                byte 2 - lowByte(currentStatus.freeRAM);
                byte 3 - highByte(currentStatus.freeRAM);
                byte 4 - currentStatus.batCorrection; //Battery voltage correction (%)
                byte 5 - currentStatus.spark; //Spark related bitfield
                byte 6 - currentStatus.O2_2; //Second O2
                byte 7 - unused;
                Can address 3104:

                byte 1 - lowByte(currentStatus.rpmDOT);
                byte 2 - highByte(currentStatus.rpmDOT);
                byte 3 - currentStatus.flex; //Flex sensor value (or 0 if not used)
        */
        // ================= CAN 0x3100 =================
        virtual String getcurrentStatus_secl(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_squirt(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_engine(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_dwell(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_MAP(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_IAT(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_coolant(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_tpsADC(bool& valid = Sensor::dummy) = 0;

        // ================= CAN 0x3101 =================
        virtual String getcurrentStatus_battery10(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_O2(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_egoCorrection(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_iatCorrection(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_wueCorrection(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_RPM(bool& valid = Sensor::dummy) = 0; //HB, LB
        
        virtual String getcurrentStatus_TAEamount(bool& valid = Sensor::dummy) = 0;

        // ================= CAN 0x3102 =================
        virtual String getcurrentStatus_barometerCorrection(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_corrections(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_VE(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_afrTarget(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_PW(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_tpsDOT(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_advance(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_TPS(bool& valid = Sensor::dummy) = 0;

        // ================= CAN 0x3103 =================
        virtual String getcurrentStatus_loopsPerSecond(bool& valid = Sensor::dummy) = 0; //HB, LB
        
        virtual String getcurrentStatus_freeRAM(bool& valid = Sensor::dummy) = 0; //HB, LB
        
        virtual String getcurrentStatus_batCorrection(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_spark(bool& valid = Sensor::dummy) = 0;
        virtual String getcurrentStatus_O2_2(bool& valid = Sensor::dummy) = 0;
        //byte 7 is unused

        // ================= CAN 0x3104 =================
        virtual String getcurrentStatus_rpmDOT(bool& valid = Sensor::dummy) = 0; //HB, LB
       
        virtual String getcurrentStatus_flex(bool& valid = Sensor::dummy) = 0;
        //bytes 2-7 is unused
    
    protected:
     
        std::unordered_map<uint16_t, uint64_t> _validation_speeduinoMap;

        // Management
		uint64_t _lastUpdateTime = 0;
		bool _isAsleep = false;

        bool _validate(uint16_t id);
};
#endif








