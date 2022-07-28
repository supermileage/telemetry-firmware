PARENT_DIRECTORY = ..
SRC_DIR = $(PARENT_DIRECTORY)/src/
LIB_DIR = $(PARENT_DIRECTORY)/lib/
TEST_DIR = $(PARENT_DIRECTORY)/test/
BUILD_DIR = $(TEST_DIR)obj/
DEP_DIR = $(TEST_DIR)dep/
BIN_DIR = $(TEST_DIR)bin/

# SENSOR TEST FILES
SENSOR := System/Handleable.o System/Handler.o Sensor/Sensor.o
CAN := System/CanInterface.o Sensor/CanListener.o
BMS := Sensor/CanSensorBms.o
# Orion Bms Test Files
ORION_TEST := Sensor/CanSensorOrionBms.o tests/Sensor/CanSensorOrionBmsTest.o
# Tiny Bms Test Files
TINY_TEST := Sensor/CanSensorTinyBms.o tests/Sensor/CanSensorTinyBmsTest.o
# Steering Test Files
STEERING_TEST := Sensor/CanSensorSteering.o tests/Sensor/CanSensorSteeringTest.o
# Accessories Test Files
ACCESSORIES_TEST := Sensor/CanSensorAccessories.o tests/Sensor/CanSensorAccessoriesTest.o
# Bms Manager Test Files
BMS_MANAGER_TEST := Sensor/BmsManager.o tests/Sensor/BmsManagerTest.o
# All Test Object Files
SENSOR_TEST := $(SENSOR) $(CAN) $(BMS) $(TINY_TEST) $(ORION_TEST) $(STEERING_TEST) $(ACCESSORIES_TEST) $(BMS_MANAGER_TEST)

# ALL TEST FILES
TEST := $(SENSOR_TEST) test.o
TEST_OBJ := $(foreach %,$(TEST),$(BUILD_DIR)$(%))