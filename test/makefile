include tests/testfiles.mk

CC = g++
VERSION = -std=c++11
LIBS = -lc++ -stdlib=libc++
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)$*.d
CFLAGS = $(VERSION) $(DEPFLAGS) -Wall
LFLAGS = $(VERSION) $(LIBS) $(PARTICLE_TEST)
INCLUDE_PREFIX = -I
PARTICLE_TEST = external/UnitTestLib/libwiringgcc.a
INCLUDE_DIRS = external/UnitTestLib external/Catch2/single_include/catch2 tests/Mocks ./ ../lib/can-common/src

# Get all directories in src and add to includes
SRC_DIRS = $(sort $(dir $(wildcard $(SRC_DIR)*/)))
ifeq ($(filter $(SRC_DIR),$(SRC_DIRS)),)
	INCLUDE_DIRS += $(SRC_DIRS) $(SRC_DIR)
else
	INCLUDE_DIRS += $(SRC_DIRS)
endif

# add src files (excluding main.cpp)
SRC_SUB_DIRS = $(filter-out $(SRC_DIR),$(SRC_DIRS))
CPPSRC += $(foreach %,$(SRC_SUB_DIRS),$(wildcard $(%)*.cpp))

# Create flags to include all directories (so we don't have to use paths in #include)
INCLUDE_FLAGS := $(foreach %,$(INCLUDE_DIRS),$(INCLUDE_PREFIX)$(wildcard $(%)))

# generate object / dependency file paths for source and library files
OBJ_SRC := $(patsubst $(SRC_DIR)%,$(BUILD_DIR)%,$(CPPSRC:.cpp=.o))
OBJ = $(patsubst $(LIB_DIR)%,$(BUILD_DIR)%,$(OBJ_SRC))
DEPENDENCIES := $(patsubst $(BUILD_DIR)%.o,$(DEP_DIR)%.d,$(OBJ))

test : $(TEST_OBJ) $(BIN_DIR) libwiringgcc
	@echo ' *** Linking $@ *** '
	@g++ $(LFLAGS) $(TEST_OBJ) $(INCLUDE_FLAGS) -o $(BIN_DIR)$@

libwiringgcc :
	@echo ' *** building $@ *** '
	cd external/UnitTestLib && make libwiringgcc.a

.PHONY: clean

clean : 
	@rm -r obj
	@rm -r bin
	@rm -r dep

# rules for generating object and dependency files
$(BUILD_DIR)%.o: $(SRC_DIR)%.cpp $(DEP_DIR)%.d | $(DEP_DIR)
	$(call compile,$<,$(@D),$@)

$(BUILD_DIR)%.o: $(LIB_DIR)%.cpp $(DEP_DIR)%.d | $(DEP_DIR)
	$(call compile,$<,$(@D),$@)

$(BUILD_DIR)%.o: $(TEST_DIR)%.cpp $(DEP_DIR)%.d | $(DEP_DIR)
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