#ifndef _BMS_FAULT_H_
#define _BMS_FAULT_H_

namespace BmsFault {
    enum Code {
        NONE=0,
        VOLTAGE_LOW,
        VOLTAGE_HIGH, 
        VOLTAGE_VERY_HIGH,
        TEMP_LOW,
        TEMP_HIGH, 
        TEMP_BMS_HIGH,
        CURRENT_DISCHARGE_HIGH,
        CURRENT_CHARGE_HIGH,
        CURRENT_REGEN_HIGH,
        SWITCH_CHARGE,
        SWITCH_DISCHARGE,
        CURRENT_SENSOR_DISCONNECT,
        CURRENT_SENSOR_CONNECT,
        SOFTWARE,
        HARDWARE,
        CELL_ASIC,
        OPEN_WIRING,
        WEAK_CELL,
        WEAK_PACK,
        BALANCING,
        COMM_INTERNAL,
        COMM_EXTERNAL,
        POWER_SUPPLY,
        HIGH_VOLTAGE_ISOLATION,
        THERMISTOR    
    };

    inline String toString(uint8_t code) {
        switch(code) {
            case NONE:
                return "None";
            case VOLTAGE_LOW:
                return "Low Voltage";
            case VOLTAGE_HIGH:
                return "High Voltage";
            case VOLTAGE_VERY_HIGH:
                return "Very High Voltage";
            case TEMP_LOW:
                return "Low Temperature";
            case TEMP_HIGH:
                return "High Temperature";
            case TEMP_BMS_HIGH:
                return "High BMS Temperature";
            case CURRENT_DISCHARGE_HIGH:
                return "High Discharge Current";
            case CURRENT_CHARGE_HIGH:
                return "High Charge Current";
            case CURRENT_REGEN_HIGH:
                return "High Regen Current";
            case SWITCH_CHARGE:
                return "Charge Switch";
            case SWITCH_DISCHARGE:
                return "Discharge Switch";
            case CURRENT_SENSOR_DISCONNECT:
                return "Current Sensor Disconnected";
            case CURRENT_SENSOR_CONNECT:
                return "Current Sensor Connected";
            case SOFTWARE:
                return "Software";
            case HARDWARE:
                return "Hardware";
            case CELL_ASIC:
                return "Cell ASIC";
            case OPEN_WIRING:
                return "Open Wiring";
            case WEAK_CELL:
                return "Weak Cell";
            case WEAK_PACK:
                return "Weak Pack";
            case BALANCING:
                return "Balancing";
            case COMM_INTERNAL:
                return "Internal Communication";
            case COMM_EXTERNAL:
                return "External Communication";
            case POWER_SUPPLY:
                return "Power Supply";
            case HIGH_VOLTAGE_ISOLATION:
                return "High Voltage Isolation";
            case THERMISTOR:
                return "Thermistor";
            default:
                return "Unknown";

        }
    }

};

#endif
