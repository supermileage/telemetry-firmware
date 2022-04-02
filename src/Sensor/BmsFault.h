#ifndef _BMS_FAULT_H_
#define _BMS_FAULT_H_

namespace BmsFault {
    enum Code {
        NONE=0,
        TEMP_HIGH, 
        CELL_OVER_5V,
        VOLTAGE_HIGH, 
        CURRENT_DISCHARGE_HIGH,
        CURRENT_CHARGE_HIGH,
        SWITCH_CHARGE,
        SWITCH_DISCHARGE,
        OPEN_WIRING,
        CURRENT_SENSOR_DISCONNECT,
        HARDWARE,
        INTERNAL_LOGIC,
        COMMUNICATION_INTERNAL,
        CELL_BANK,    
        BALANCING,
        THERMISTOR_PACK,
        THERMISTOR_INTERNAL, 
        COMMUNICATION_CAN,
        VOLTAGE_LOW,
        TEMP_LOW,
        WEAK_PACK,
        WEAK_CELL,
        CURRENT_SENSOR_CONNECT
    };

    inline String toString(uint8_t code) {
        switch(code) {
            case NONE:
                return "None";
            case TEMP_HIGH:
                return "High Temperature";
            case CELL_OVER_5V:
                return "Cell Voltage >5v";
            case VOLTAGE_HIGH:
                return "High Voltage";
            case CURRENT_DISCHARGE_HIGH:
                return "High Discharge Current";
            case CURRENT_CHARGE_HIGH:
                return "High Charge Current";
            case SWITCH_CHARGE:
                return "Charge Switch";
            case SWITCH_DISCHARGE:
                return "Discharge Switch";
            case OPEN_WIRING:
                return "Open Wiring";
            case CURRENT_SENSOR_DISCONNECT:
                return "Current Sensor Disconnected";
            case HARDWARE:
                return "Hardware";
            case INTERNAL_LOGIC:
                return "Internal Logic";
            case COMMUNICATION_INTERNAL:
                return "Internal Communication";
            case CELL_BANK:
                return "Cell Bank";
            case BALANCING:
                return "Balancing";
            case THERMISTOR_PACK:
                return "Battery Pack Thermistor";
            case THERMISTOR_INTERNAL:
                return "BMS Thermistor";
            case COMMUNICATION_CAN:
                return "CAN Communication";
            case VOLTAGE_LOW:
                return "Low Voltage";
            case TEMP_LOW:
                return "Low Temperature";
            case WEAK_PACK:
                return "Weak Pack";
            case WEAK_CELL:
                return "Weak Cell";
            case CURRENT_SENSOR_CONNECT:
                return "Current Sensor Connected";
            default:
                return "Unknown";

        }
    }

};

#endif
