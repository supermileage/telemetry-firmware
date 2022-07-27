
test : test.cpp ../src/System/CanInterface.cpp ../src/System/Handleable.cpp ../src/System/Handler.cpp ../src/System/CanInterface.h ../src/System/Handleable.h ../src/System/Handler.h libwiringgcc
	g++ test.cpp ../src/System/CanInterface.cpp ../src/System/Handleable.cpp ../src/System/Handler.cpp External/UnitTestLib/libwiringgcc.a -std=c++11 -lc++ -stdlib=libc++ -IExternal/UnitTestLib -I../src -I../src/System -I../src/Sensor -I../src/Logging -IMock -o test

libwiringgcc :
	cd External/UnitTestLib && make libwiringgcc.a