#include "Particle.h"
#include "CanInterface.h"
#include "CanBusMock.h"

#include <iostream>
#include <stdint.h>

int main(int argc, char** argv) {

	CanBusMock mock(1);
	CanInterface interface(&mock);

	interface.begin();
	interface.handle();

	std::cout << "Hello tests!" << std::endl;
} 