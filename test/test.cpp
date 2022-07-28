#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

int main (int argc, char* argv[]) {
	Catch::Session session;

	auto exitCode = session.applyCommandLine(argc, argv);
	if (exitCode != 0)
		return exitCode;

	return session.run();
}