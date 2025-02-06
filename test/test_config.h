#ifndef _TEST_CONFIG_H_
#define _TEST_CONFIG_H_

#include "catch.hpp"
#include "settings.h"
#include "Sensor.h"

// Can
#define CAN_MESSAGE_AVAIL_TEST 5

// Validation
#define DEFAULT_START_TIME_MILLIS 5000
#define DEFAULT_STALE_TIME_MILLIS DEFAULT_START_TIME_MILLIS + STALE_INTERVAL

#endif