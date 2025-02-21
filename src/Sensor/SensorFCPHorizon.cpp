#include "SensorFcpHorizon.h"
#include "fcp-common.h"
#include "settings.h"

#define DEBUG_FCP_Horizon   // For debugging FCP Horizon Sensor, comment out in production plz

#ifdef DEBUG_FCP_Horizon
#define FC_DEBUG_INTERVAL 100
uint32_t sensor_fcp_horizon_last_debug_output = 0;
#endif

#define FC_PACKET_LENGTH 8 // Horizon Fuel Cell packet length is 8 bytes

// See Notion for Horizon Fuel Cell Controller documentation
#define AMBIENT_TEMP_UNIT 0.5         // Degrees Celcius
#define FUEL_CELL_VOLTAGE_UNIT 0.333  // Volts
#define H2_LEAK_VOLTAGE_UNIT 0.1      // Volts
#define FUEL_CELL_TEMP_UNIT 0.5       // Degrees Celcius
#define FUEL_CELL_CURRENT_UNIT 0.2    // Amps
#define BATTERY_VOLTAGE_UNIT 0.1      // Volts

// Constructor for Debug Configuration
SensorFcpHorizon::SensorFcpHorizon(TelemetrySerial* serial) : _serial(serial) { }

// Default constructor
SensorFcpHorizon::SensorFcpHorizon() {}

// Destructor
SensorFcpHorizon::~SensorFcpHorizon() { }

// Get Human Name
String SensorFcpHorizon::getHumanName() {
    return "FCP Horizon Control";
}

void SensorFcpHorizon::begin() {
    // Horizon Usart baud rate: 9600
    // Horizon Data frame format: SERIAL_8N1, 8 data bits, 1 stop bit, no flow control
    // Usart is asychronous, must have same baud rate as Horizon controller
    _serial->begin(FC_HORIZON_BAUD, SERIAL_8N1);
}

void SensorFcpHorizon::handle() {
    // check if the data is stale
    if (millis() < _lastUpdate + STALE_INTERVAL) {
        _valid = true;
    } else {
        _valid = false;
    }

    // Check if there are bytes available
    int bytesAvailable = _serial->available();
    if (bytesAvailable < FC_PACKET_LENGTH) {
        #ifdef DEBUG_FCP_Horizon
        // If not enough available bytes, print debug message. 
        if (bytesAvailable > 0 && millis() >= sensor_fcp_horizon_last_debug_output + FC_DEBUG_INTERVAL) {
            sensor_fcp_horizon_last_debug_output = millis();
            DEBUG_SERIAL_F("Received %d bytes, but expect %d bytes from FCP Horizon\n", _serial->available(), FC_PACKET_LENGTH);
        }
        #endif
        return;
    }

    // Read the data packets from serial buffer
    uint8_t dataBuffer[FC_PACKET_LENGTH] = { 0 };
    _serial->readBytes((char*)dataBuffer, FC_PACKET_LENGTH); // Cast to char *

    #ifdef DEBUG_FCP_Horizon
    // Debug output for the data packets received
    DEBUG_SERIAL_LN("-----------------------------");
    DEBUG_SERIAL("SensorFcpHorizon Received Message:");
    for (int i = 0; i < FC_PACKET_LENGTH; i++) {
        DEBUG_SERIAL_F("0x%x ", dataBuffer[i]);
    }
    DEBUG_SERIAL_LN();
    #endif

    // Unpack the data packets
    _unpackData(dataBuffer);
}

void SensorFcpHorizon::_unpackData(uint8_t* buf) {
    // See Notion for Horizon Fuel Cell Controller documentation
    _ambientTemperature = buf[1] * AMBIENT_TEMP_UNIT;
    _fuelCellVoltage = buf[2] * FUEL_CELL_VOLTAGE_UNIT;
    _h2LeakVoltage = buf[3] * H2_LEAK_VOLTAGE_UNIT;
    _fuelCellTemperature = buf[4] * FUEL_CELL_TEMP_UNIT;
    _fuelCellCurrentHigh = buf[5] * FUEL_CELL_CURRENT_UNIT;
    _fuelCellCurrentLow = buf[6] * FUEL_CELL_CURRENT_UNIT;
    _batteryVoltage = buf[7] * BATTERY_VOLTAGE_UNIT;

    // Mark the data as valid and update the last update timestamp
    _valid = true;
    _lastUpdate = millis();

    #ifdef DEBUG_FCP_Horizon
    // Debug output for unpacked data
    DEBUG_SERIAL_F("Ambient Temperature from buffer: %.1f C\n", _ambientTemperature);
    DEBUG_SERIAL_F("Fuel Cell Voltage from buffer: %.3f V\n", _fuelCellVoltage);
    DEBUG_SERIAL_F("H2 Leak Voltage from buffer: %.1f V\n", _h2LeakVoltage);
    DEBUG_SERIAL_F("Fuel Cell Temperature from buffer: %.1f C\n", _fuelCellTemperature);
    DEBUG_SERIAL_F("Fuel Cell Current High from buffer: %.1f A\n", _fuelCellCurrentHigh);
    DEBUG_SERIAL_F("Fuel Cell Current Low from buffer: %.1f A\n", _fuelCellCurrentLow);
    DEBUG_SERIAL_F("Battery Voltage from buffer: %.1f V\n", _batteryVoltage);
    #endif
}

// Flush the serial buffer
void SensorFcpHorizon::_flushSerial() {
	while (_serial->available()) {
		_serial->read();
	}
}
