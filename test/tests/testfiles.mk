ROOT_DIRECTORY = .
SRC_DIR = $(ROOT_DIRECTORY)/src/
LIB_DIR = $(ROOT_DIRECTORY)/lib/
TEST_DIR = $(ROOT_DIRECTORY)/test/
BUILD_DIR = $(TEST_DIR)obj/
DEP_DIR = $(TEST_DIR)dep/
BIN_DIR = $(TEST_DIR)bin/

# SENSOR TEST FILES
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

# All Test Object Files
TEST_DIRS := $(dir $(wildcard $(TEST_DIR)tests/*/))
TEST_SRC := $(patsubst $(TEST_DIR)%,%,$(foreach %,$(TEST_DIRS),$(wildcard $(%)*.cpp)))
SENSOR_SRC := $(SENSOR) $(CAN) $(BMS) $(TINY) $(ORION) $(STEERING) $(ACCESSORIES) $(BMS_MANAGER)

# ALL TEST FILES
TEST := $(SENSOR_SRC) $(TEST_SRC) test.cpp
TEST_OBJ := $(patsubst %.cpp,%.o,$(foreach %,$(TEST),$(BUILD_DIR)$(%)))

# ALL TEST INCLUDE DIRS
TEST_DIRS += $(TEST_DIR)