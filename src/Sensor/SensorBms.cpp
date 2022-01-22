#include "SensorBms.h"

#define PARAM_ID_PACK_VOLTAGE       0x14
#define PARAM_ID_PACK_CURRENT       0x15
#define PARAM_ID_MAX_CELL_VOLTAGE   0x16
#define PARAM_ID_MIN_CELL_VOLTAGE   0x17
#define PARAM_ID_STATUS             0x18
#define PARAM_ID_SOC                0x1A
#define PARAM_ID_TEMP               0x1B

#define TEMP_ID_INTERNAL            0x00
#define TEMP_ID_BATTERY_1           0x01
#define TEMP_ID_BATTERY_2           0x02

void SensorBms::begin() {

}

void SensorBms::handle() {
    
}

String SensorBms::getHumanName() {
    return "BMS";
}

String SensorBms::getSoc() {
    return "0.0"; 
}

String SensorBms::getBatteryVolt() {
    return "0.0";
}

String SensorBms::getBatteryCurrent() {
    return "0.0";
}

String SensorBms::getMaxVolt() {
    return "0.0";
}

String SensorBms::getMinVolt() {
    return "0.0";
}

String SensorBms::getStatusBMS() {
    return "0.0";
}

int SensorBms::getTempBMS() {
    return 0;
}

int SensorBms::getBatteryTemp1() {
    return 0;
}

int SensorBms::getBatteryTemp2() {
    return 0;
}