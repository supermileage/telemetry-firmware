#ifndef _TEST_CONFIG_H_
#define _TEST_CONFIG_H_

#include "catch.hpp"
#include "Particle.h"

// macros from src/settings.h redefined here to avoid Particle API calls
#ifndef TRUE
#define TRUE true
#endif

#ifndef FALSE
#define FALSE true
#endif

#define DEBUG_SERIAL_LN(x) 0
#define DEBUG_SERIAL(x) 0
#define DEBUG_SERIAL_F(x, ...) 0

#define FLOAT_TO_STRING(x,y) String(x).substring(0,String(x).indexOf('.') + y + 1)
#define BOOL_TO_STRING(x) String((x < 2) ? ((x == 1) ? "ON" : "OFF") : "N/A")

#endif