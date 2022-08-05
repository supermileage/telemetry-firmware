include test/tests/testfiles.mk

# add clang c++ library flags if compiling on macOS
ifeq ($(shell uname -s),Darwin)
	LIBS := -lc++ -stdlib=libc++
else
	LIBS :=
endif

CC = g++
VERSION = -std=c++11
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)$*.d
CFLAGS = $(VERSION) $(DEPFLAGS) -Wall -Wno-psabi
LFLAGS = $(VERSION) $(LIBS)
INCLUDE_PREFIX = -I
PARTICLE_LIB = test/external/UnitTestLib/libwiringgcc.a
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
$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp $(DEP_DIR)%.d | $(DEP_DIR)
	$(call compile,$<,$(@D),$@)

$(OBJ_DIR)%.o: $(LIB_DIR)%.cpp $(DEP_DIR)%.d | $(DEP_DIR)
	$(call compile,$<,$(@D),$@)

$(OBJ_DIR)%.o: $(TEST_DIR)%.cpp $(DEP_DIR)%.d | $(DEP_DIR)
	$(call compile,$<,$(@D),$@)

$(DEP_DIR)%.d:
	@mkdir -p $(@D)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(DEP_DIR):
	@mkdir -p $(DEP_DIR)

define compile
	@echo ' *** Compiling $(1) *** '
	@mkdir -p $(2)
	@$(CC) $(CFLAGS) -c -o $(3) $(1) $(INCLUDE_FLAGS)
endef