#ifndef _BMS_FAULT_H_
#define _BMS_FAULT_H_

namespace BmsFault {
    enum Code {
        NONE=0,
        VOLTAGE_LOW,
        VOLTAGE_HIGH, 
        CELL_OVER_5V,
        TEMP_LOW,
        TEMP_HIGH, 
        TEMP_BMS_HIGH,
        THERMISTOR_PACK,
        THERMISTOR_INTERNAL, 
        CURRENT_DISCHARGE_HIGH,
        CURRENT_CHARGE_HIGH,
        CURRENT_REGEN_HIGH,
        CURRENT_SENSOR_DISCONNECT,
        CURRENT_SENSOR_CONNECT,
        SWITCH_CHARGE,
        SWITCH_DISCHARGE,
        SOFTWARE,
        HARDWARE,
        INTERNAL_LOGIC,
        COMMUNICATION_INTERNAL,
        COMMUNICATION_CAN,
        OPEN_WIRING,
        WEAK_CELL,
        WEAK_PACK,
        BALANCING,
        POWER_SUPPLY,
        HIGH_VOLTAGE_ISOLATION,
        CELL_BANK    
    };

    inline String toString(uint8_t code) {
        switch(code) {
            case NONE:
                return "None";
            case VOLTAGE_LOW:
                return "Low Voltage";
            case VOLTAGE_HIGH:
                return "High Voltage";
            case CELL_OVER_5V:
                return "Cell Voltage >5v";
            case TEMP_LOW:
                return "Low Temperature";
            case TEMP_HIGH:
                return "High Temperature";
            case TEMP_BMS_HIGH:
                return "High BMS Temperature";
            case THERMISTOR_PACK:
                return "Battery Pack Thermistor";
            case THERMISTOR_INTERNAL,
                return "BMS THermistor"
            case CURRENT_DISCHARGE_HIGH:
                return "High Discharge Current";
            case CURRENT_CHARGE_HIGH:
                return "High Charge Current";
            case CURRENT_REGEN_HIGH:
                return "High Regen Current";
            case CURRENT_SENSOR_DISCONNECT:
                return "Current Sensor Disconnected";
            case CURRENT_SENSOR_CONNECT:
                return "Current Sensor Connected";
            case SWITCH_CHARGE:
                return "Charge Switch";
            case SWITCH_DISCHARGE:
                return "Discharge Switch";
            case SOFTWARE:
                return "Software";
            case HARDWARE:
                return "Hardware";
            case INTERNAL_LOGIC:
                return "Internal Logic";
            case COMMUNICATION_INTERNAL:
                return "Internal Communication";
            case COMMUNICATION_CAN:
                return "CAN Communication";
            case OPEN_WIRING:
                return "Open Wiring";
            case WEAK_CELL:
                return "Weak Cell";
            case WEAK_PACK:
                return "Weak Pack";
            case BALANCING:
                return "Balancing";
            case POWER_SUPPLY:
                return "Power Supply";
            case HIGH_VOLTAGE_ISOLATION:
                return "High Voltage Isolation";
            case CELL_BANK:
                return "Cell Bank";
            default:
                return "Unknown";

        }
    }

};

#endif
