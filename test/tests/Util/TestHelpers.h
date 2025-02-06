#ifndef _TEST_HELPERS_H_
#define _TEST_HELPERS_H_

#include "test_config.h"

template <typename T, typename R>
void testValidation(T& object, R (T::*getter)(bool&)) {
	bool statusIsValid = false;
	setMillis(DEFAULT_STALE_TIME_MILLIS - 1);
	(object.*getter)(statusIsValid);
	REQUIRE ( statusIsValid );

	setMillis(DEFAULT_STALE_TIME_MILLIS);
	(object.*getter)(statusIsValid);
	REQUIRE_FALSE ( statusIsValid );
}

#endif