SRC_DIR = src/
LIB_DIR = lib/
TEST_DIR = test/
BUILD_DIR = $(TEST_DIR)obj/
DEP_DIR = $(TEST_DIR)dep/
BIN_DIR = $(TEST_DIR)bin/

# SRC TEST FILES -- Only these files will be compiled for tests
SENSOR := System/Handleable.cpp System/Handler.cpp Sensor/Sensor.cpp
CAN := System/CanInterface.cpp Sensor/CanListener.cpp
BMS := Sensor/CanSensorBms.cpp
# Orion Bms Test Files
ORION := Sensor/CanSensorOrionBms.cpp
# Tiny Bms Test Files
TINY := Sensor/CanSensorTinyBms.cpp
# Steering Test Files
STEERING := Sensor/CanSensorSteering.cpp
# Accessories Test Files
ACCESSORIES := Sensor/CanSensorAccessories.cpp
# Bms Manager Test Files
BMS_MANAGER := Sensor/BmsManager.cpp

# All files to compile for tests
TEST_DIRS := $(dir $(wildcard $(TEST_DIR)tests/*/))
CPPTEST := $(patsubst $(TEST_DIR)%,%,$(foreach %,$(TEST_DIRS),$(wildcard $(%)*.cpp)))
CPPSRC_TEMP := $(SENSOR) $(CAN) $(BMS) $(TINY) $(ORION) $(STEERING) $(ACCESSORIES) $(BMS_MANAGER)
CPPSRC_TEST := $(foreach %,$(CPPSRC_TEMP),$(SRC_DIR)$(%))

# ALL TEST FILES
TEST := $(CPPSRC_TEMP) $(CPPTEST) test.cpp
TEST_OBJ := $(patsubst %.cpp,%.o,$(foreach %,$(TEST),$(BUILD_DIR)$(%)))

# ALL TEST INCLUDE DIRS
TEST_DIRS += $(TEST_DIR)