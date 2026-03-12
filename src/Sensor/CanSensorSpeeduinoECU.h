#ifndef _CAN_SENSOR_SPEEDUINO_ECU_H_
#define _CAN_SENSOR_SPEEDUINO_ECU_H_
#include "CanSensorSpeeduinoECUtemp.h"
#include "CanInterface.h"
#include "can_common.h"


class CanSensorSpeeduinoECU : public CanSensorSpeeduinoECUtemp {
	public:

        CanSensorSpeeduinoECU(CanInterface& canInterface);
        ~CanSensorSpeeduinoECU();

        void begin() override;

        String getHumanName() override;

		// Constructors
		String getcurrentStatus_secl(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_squirt(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_engine(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_dwell(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_MAP(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_IAT(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_coolant(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_tpsADC(bool& valid = Sensor::dummy) override;

        // ================= CAN 0x3101 =================
        String getcurrentStatus_battery10(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_O2(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_egoCorrection(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_iatCorrection(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_wueCorrection(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_RPM(bool& valid = Sensor::dummy) override;
        
        String getcurrentStatus_TAEamount(bool& valid = Sensor::dummy) override;

        // ================= CAN 0x3102 =================
        String getcurrentStatus_barometerCorrection(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_corrections(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_VE(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_afrTarget(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_PW(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_tpsDOT(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_advance(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_TPS(bool& valid = Sensor::dummy) override;

        // ================= CAN 0x3103 =================
        String getcurrentStatus_loopsPerSecond(bool& valid = Sensor::dummy) override;
        
        String getcurrentStatus_freeRAM(bool& valid = Sensor::dummy) override;
        
        String getcurrentStatus_batCorrection(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_spark(bool& valid = Sensor::dummy) override;
        String getcurrentStatus_O2_2(bool& valid = Sensor::dummy) override;

        // ================= CAN 0x3104 =================
        String getcurrentStatus_rpmDOT(bool& valid = Sensor::dummy) override;
        
        String getcurrentStatus_flex(bool& valid = Sensor::dummy) override;

        uint64_t getLastUpdateTime();

   

	private:
        float _secl = 0;
        float _squirt = 0;
        float _engine = 0;
        float _dwell = 0;
        float _MAP = 0;
        float _IAT = 0;
        float _coolant = 0;
        float _tpsADC = 0;

        // ================= CAN 0x3101 =================
        float _battery10 = 0;
        float _O2 = 0;
        float _egoCorrection = 0;
        float _iatCorrection = 0;
        float _wueCorrection = 0;
        float _RPM = 0;
        float _TAEamount = 0;

        // ================= CAN 0x3102 =================
        float _barometerCorrection = 0;
        float _corrections = 0;   // Total GammaE (%)
        float _VE = 0;            // Current VE 1 (%)
        float _afrTarget = 0;
        float _PW = 0;            // Pulsewidth 1
        float _tpsDOT = 0;        // TPS DOT
        float _advance = 0;
        float _TPS = 0;           // TPS (0% to 100%)


        // ================= CAN 0x3103 =================
        float _loopsPerSecond_3103 = 0;
        float _freeRAM_3103 = 0;
        float _batCorrection_3103 = 0;
        float _spark_3103 = 0;
        float _O2_2_3103 = 0;

        // ================= CAN 0x3104 =================
        float _rpmDOT = 0;
        float _flex = 0;

		/**
         * @brief Called by delegate in CanInterface when a message with one of Orion's ids is receieved
         * 
         * @param message CanMessage received from CanInterface with OrionBms data
         */
		void update(CanMessage message) override;

        //parses states here if needed

        
		/**
		 * @brief parses a big-endian two byte signed integer from buffer
		 * 
		 * @param buf pointer to first byte to be read (left to right)
		 */
        
};
#endif