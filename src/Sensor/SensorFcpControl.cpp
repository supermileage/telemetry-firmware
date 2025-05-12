#include "SensorFcpControl.h"
#include "fcp-common.h"
#include "settings.h"
#include "Particle.h"

#define DEBUG_FCP_CONTROL   // For debugging FCP Horizon Sensor, comment out in production plz

#ifdef DEBUG_FCP_CONTROL
#define FC_DEBUG_INTERVAL 100
uint32_t last_debug_output = 0;
#endif

#define FC_PACKET_LENGTH 8 // Horizon Fuel Cell packet length is 8 bytes

// See Notion for Horizon Fuel Cell Controller documentation
#define AMBIENT_TEMP_UNIT 0.5         // Degrees Celcius
#define FUEL_CELL_VOLTAGE_UNIT 0.333  // Volts
#define H2_LEAK_VOLTAGE_UNIT 0.1      // Volts
#define FUEL_CELL_TEMP_UNIT 0.5       // Degrees Celcius
#define FUEL_CELL_CURRENT_UNIT 0.2    // Amps
#define BATTERY_VOLTAGE_UNIT 0.1      // Volts

// Error Flags
#define ERROR_MANUAL_SHUTDOWN 20
#define ERROR_VOLTAGE_LOW 2
#define ERROR_H2_LEAK 3
#define ERROR_CURRENT_HIGH 4
#define ERROR_TEMP_HIGH 5
#define ERROR_VOLTAGE_LOW_CURRENT_HIGH 6


// Constructor for Debug Configuration
SensorFcpControl::SensorFcpControl(TelemetrySerial* serial) : _serial(serial) { }

// Default constructor
SensorFcpControl::SensorFcpControl() {}

// Destructor
SensorFcpControl::~SensorFcpControl() { }

// Get Human Name
String SensorFcpControl::getHumanName() {
    return "FCP Horizon Control";
}

void SensorFcpControl::begin() {
    // Horizon Usart baud rate: 9600
    // Horizon Data frame format: SERIAL_8N1, 8 data bits, 1 stop bit, no flow control
    // Usart is asychronous, must have same baud rate as Horizon controller
    _serial->begin(FC_BAUD, SERIAL_8N1);
    _serial->setTimeout(FC_TIMEOUT);
    _flushSerial(); // Flush the serial buffer to clean slate

    #ifdef DEBUG_FCP_CONTROL
    DEBUG_SERIAL_F("FCP: Serial port initialized. Baud rate: %d", FC_BAUD);
    #endif
}

void SensorFcpControl::handle() {
    // Check if the data is stale
    if (millis() < _lastUpdate + STALE_INTERVAL) {
        _valid = true;
    } else {
        _valid = false;
    }

    int bytesAvailable = _serial->available();

    #ifdef DEBUG_FCP_CONTROL
    static uint32_t lastBytesDebug = 0;
    if (bytesAvailable == 0 && millis() - lastBytesDebug >= 1000) {
        lastBytesDebug = millis();
        DEBUG_SERIAL_LN("FCP: No bytes available");
    } else if (bytesAvailable > 0 && bytesAvailable < FC_PACKET_LENGTH) {
        if (millis() - lastBytesDebug >= FC_DEBUG_INTERVAL) {
            lastBytesDebug = millis();
            DEBUG_SERIAL_F("FCP: Incomplete packet (%d bytes available)\n", bytesAvailable);
        }
    }
    #endif


    // Not enough bytes for a complete packet
    if (bytesAvailable < FC_PACKET_LENGTH) {
        return;
    }

    // Read the data packets from serial buffer
    uint8_t dataBuffer[FC_PACKET_LENGTH] = { 0 };
    size_t bytesRead =_serial->readBytes((char*)dataBuffer, FC_PACKET_LENGTH); // Cast to char *

    if (bytesRead != FC_PACKET_LENGTH) {
        #ifdef DEBUG_FCP_CONTROL
        DEBUG_SERIAL_F("FCP: Read error! Got only %d of %d bytes\n", bytesRead, FC_PACKET_LENGTH);
        #endif

        _flushSerial(); // Clear buffer for resynchronization
        return;
    }

    #ifdef DEBUG_FCP_CONTROL
    // Debug output for the data packets received
    DEBUG_SERIAL_LN("-----------------------------");
    DEBUG_SERIAL("FCP Received Message:");
    for (int i = 0; i < FC_PACKET_LENGTH; i++) {
        DEBUG_SERIAL_F("0x%x ", dataBuffer[i]);
    }
    DEBUG_SERIAL_LN();
    #endif

    // Unpack the data packets
    _unpackData(dataBuffer);
}

void SensorFcpControl::_unpackData(uint8_t* buf) {
    // See Notion for Horizon Fuel Cell Controller documentation
    _errorFlag = buf[0];
    _ambientTemperature = buf[1] * AMBIENT_TEMP_UNIT;
    _fuelCellVoltage = buf[2] * FUEL_CELL_VOLTAGE_UNIT;
    _h2LeakVoltage = buf[3] * H2_LEAK_VOLTAGE_UNIT;
    _fuelCellTemperature = buf[4] * FUEL_CELL_TEMP_UNIT;
    _fuelCellCurrentHigh = buf[5];
    _fuelCellCurrentLow = buf[6];
    _batteryVoltage = buf[7] * BATTERY_VOLTAGE_UNIT;

    // Mark the data as valid and update the last update timestamp
    _valid = true;
    _lastUpdate = millis();

    #ifdef DEBUG_FCP_CONTROL
    // Debug output for unpacked data
    DEBUG_SERIAL_F("Error Flag: 0x%x\n", _errorFlag);
    DEBUG_SERIAL_F("Ambient Temperature from buffer: %.1f C\n", _ambientTemperature);
    DEBUG_SERIAL_F("Fuel Cell Voltage from buffer: %.3f V\n", _fuelCellVoltage);
    DEBUG_SERIAL_F("H2 Leak Voltage from buffer: %.1f V\n", _h2LeakVoltage);
    DEBUG_SERIAL_F("Fuel Cell Temperature from buffer: %.1f C\n", _fuelCellTemperature);
    float fullCurrent = (_fuelCellCurrentHigh * 256 + _fuelCellCurrentLow) * FUEL_CELL_CURRENT_UNIT;
    DEBUG_SERIAL_F("Fuel Cell Current (combined): %.1f A\n", fullCurrent);
    DEBUG_SERIAL_F("Battery Voltage from buffer: %.1f V\n", _batteryVoltage);
    #endif
}

String SensorFcpControl::getAmbientTemperature(bool& valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_ambientTemperature, 1);
}

String SensorFcpControl::getFuelCellVoltage(bool& valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_fuelCellVoltage, 2);
}

String SensorFcpControl::getH2LeakVoltage(bool& valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_h2LeakVoltage, 1);
}

String SensorFcpControl::getFuelCellTemperature(bool& valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_fuelCellTemperature, 1);
}

String SensorFcpControl::getFuelCellCurrent(bool& valid) {
    valid = _valid;
    // Combine high and low bytes of current bytes
    float combinedCurrent = (_fuelCellCurrentHigh * 256 + _fuelCellCurrentLow) * FUEL_CELL_CURRENT_UNIT;
    return FLOAT_TO_STRING(combinedCurrent, 1);
}

String SensorFcpControl::getBatteryVoltage(bool& valid) {
    valid = _valid;
    return FLOAT_TO_STRING(_batteryVoltage, 1);
}

String SensorFcpControl::getErrorFlag(bool& valid) {
    valid = _valid;
    return INT_TO_STRING(_errorFlag);
}

// Flush the serial buffer
void SensorFcpControl::_flushSerial() {
	int bytesCleared = 0;
    while (_serial->available()) {
        _serial->read();
        bytesCleared++;
        
        // Avoid buffer filling up too much
        if (bytesCleared > 100) {
            break;
        }
    }
    
    #ifdef DEBUG_FCP_CONTROL
    if (bytesCleared > 0) {
        DEBUG_SERIAL_F("FCP: Flushed %d bytes from buffer\n", bytesCleared);
    }
    #endif
}
