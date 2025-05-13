/* Telemetry Macros */
#define FC_BAUD 9600
#define FC_TIMEOUT 200

#define CELL_ARRAY_SIZE 20
#define LEADING_ZEROES 0
#define TRAILING_ZEROES 1

// #define FC_NUM_CELLS (CELL_ARRAY_SIZE - LEADING_ZEROES - TRAILING_ZEROES)

// #define FC_NUM_HEADERS 6

// #define FC_HEADER_0 0x7
// #define FC_HEADER_1 0xD
// #define FC_HEADER_2 0x11
// #define FC_HEADER_3 0x17
// #define FC_HEADER_4 0x1D
// #define FC_HEADER_5 0x1F

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
