
IMAGE := ghcr.io/supermileage/particle-v3.0.0:latest
OUTPUT_DIR := output

include src/build.mk
include test/test.mk

CPPSRC := $(TEST_CPP)

$(info TEST_CPP is $(TEST_CPP))
$(info TEST_OBJ is $(TEST_OBJ))
$(info SRC_DIR is $(SRC_DIR))
$(info TEST_DIR is $(TEST_DIR))
$(info OBJ_DIR is $(OBJ_DIR))
$(info BIN_DIR is $(BIN_DIR))
$(info DEP_DIR is $(DEP_DIR))

SRC_DIR = src/
LIB_DIR = lib/
TEST_DIR = test/
OBJ_DIR = $(TEST_DIR)obj/
DEP_DIR = $(TEST_DIR)dep/
BIN_DIR = $(TEST_DIR)bin/

# testfiles.mk

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
TEST_CPP_TEMP := $(patsubst $(TEST_DIR)%,%,$(foreach %,$(TEST_DIRS),$(wildcard $(%)*.cpp)))
SRC_CPP_TEMP := $(SENSOR) $(CAN) $(BMS) $(TINY) $(ORION) $(STEERING) $(ACCESSORIES) $(BMS_MANAGER)

# ALL TEST FILES
CPP_TO_OBJ := $(SRC_CPP_TEMP) $(TEST_CPP_TEMP) test.cpp
TEST_OBJ := $(patsubst %.cpp,%.o,$(foreach %,$(CPP_TO_OBJ),$(OBJ_DIR)$(%)))
TEST_CPP := $(foreach %,$(SRC_CPP_TEMP),$(SRC_DIR)$(%)) $(foreach %,$(TEST_CPP_TEMP),$(TEST_DIR)$(%))  $(TEST_DIR)test.cpp

# ALL TEST INCLUDE DIRS
TEST_DIRS += $(TEST_DIR)

# testfiles.mk end

# test.mk

CC = g++
VERSION = -std=c++17
LIBS = -lc++ -stdlib=libc++
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)$*.d
CFLAGS = $(VERSION) $(DEPFLAGS) -Wall -Wno-psabi
LFLAGS = $(VERSION) $(LIBS) $(PARTICLE_TEST)
INCLUDE_PREFIX = -I
PARTICLE_TEST = test/external/UnitTestLib/libwiringgcc.a
INCLUDE_DIRS = test/external/UnitTestLib test/external/Catch2/single_include/catch2 lib/can-common/src $(TEST_DIRS)

# Get all directories in src and add to includes
SRC_DIRS = $(sort $(dir $(wildcard $(SRC_DIR)*/)))
ifeq ($(filter $(SRC_DIR),$(SRC_DIRS)),)
	INCLUDE_DIRS += $(SRC_DIRS) $(SRC_DIR)
else
	INCLUDE_DIRS += $(SRC_DIRS)
endif

# Create flags to include all directories (so we don't have to use paths in #include)
INCLUDE_FLAGS := $(foreach %,$(INCLUDE_DIRS),$(INCLUDE_PREFIX)$(wildcard $(%)))

# generate dependency file paths for test object files
DEPENDENCIES := $(patsubst $(OBJ_DIR)%.o,$(DEP_DIR)%.d,$(TEST_OBJ))

# rules for generating object and dependency files
$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(DEP_DIR)%.d | $(DEP_DIR)
	$(call compile,$<,$(@D),$@)

$(OBJ_DIR)%.o: $(LIB_DIR)%.cpp $(DEP_DIR)%.d | $(DEP_DIR)
	$(call compile,$<,$(@D),$@)

$(OBJ_DIR)%.o: $(TEST_DIR)%.cpp $(DEP_DIR)%.d | $(DEP_DIR)
	$(call compile,$<,$(@D),$@)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(DEP_DIR)%.d:
	@mkdir -p $(@D)

define compile
	@echo ' *** Compiling $(1) *** '
	@mkdir -p $(2)
	@$(CC) $(CFLAGS) -c -o $(3) $(1) $(INCLUDE_FLAGS)
endef

-include $(DEPENDENCIES)

# test.mk end

.PHONY: urban proto pull-image clean clean-test

urban: clean pull-image
	$(call print, COMPILING URBAN FIRMWARE)
	docker run --rm -v $(shell pwd):/app -v $(shell pwd)/$(OUTPUT_DIR):/$(OUTPUT_DIR) $(IMAGE) \
		make all PLATFORM=boron APPDIR=/app/ TARGET_DIR=/$(OUTPUT_DIR) EXTRA_CFLAGS+='-DURBAN $(EXTRA_CFLAGS)'
	$(call print, TAKING OWNERSHIP OF FILES - YOU MAY NEED YOUR PASSWORD)
	sudo chown -R $(shell id -u):$(shell id -g) $(OUTPUT_DIR)

proto: clean pull-image
	$(call print, COMPILING PROTO FIRMWARE)
	docker run --rm -v $(shell pwd):/app -v $(shell pwd)/$(OUTPUT_DIR):/$(OUTPUT_DIR) $(IMAGE) \
		make all PLATFORM=boron APPDIR=/app/ TARGET_DIR=/$(OUTPUT_DIR) EXTRA_CFLAGS+='-DPROTO $(EXTRA_CFLAGS)'
	$(call print, TAKING OWNERSHIP OF FILES - YOU MAY NEED YOUR PASSWORD)
	sudo chown -R $(shell id -u):$(shell id -g) $(OUTPUT_DIR)

fc: clean pull-image
	$(call print, COMPILING FC FIRMWARE)
	docker run --rm -v $(shell pwd):/app -v $(shell pwd)/$(OUTPUT_DIR):/$(OUTPUT_DIR) $(IMAGE) \
		make all PLATFORM=boron APPDIR=/app/ TARGET_DIR=/$(OUTPUT_DIR) EXTRA_CFLAGS+='-DFC $(EXTRA_CFLAGS)'
	$(call print, TAKING OWNERSHIP OF FILES - YOU MAY NEED YOUR PASSWORD)
	sudo chown -R $(shell id -u):$(shell id -g) $(OUTPUT_DIR)

test : $(TEST_OBJ) $(BIN_DIR) libwiringgcc
	@echo ' *** Building $@ *** '
	@g++ $(LFLAGS) $(TEST_OBJ) $(INCLUDE_FLAGS) -o $(BIN_DIR)$@

run-tests:
	@chmod +x ./test/bin/test
	./test/bin/test

pull-image:
	docker pull $(IMAGE)

libwiringgcc:
	@echo ' *** building $@ *** '
	@cd test/external/UnitTestLib && make libwiringgcc.a

clean:
	rm -rf $(OUTPUT_DIR)

clean-test:
	@rm -rf test/obj
	@rm -rf test/bin
	@rm -rf test/dep

define print
	@echo ''
	@echo '*** $(1) ***'
	@echo ''
endef