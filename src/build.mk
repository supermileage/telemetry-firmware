# Standard behavior must be included here
INCLUDE_DIRS += $(SOURCE_PATH)$(USRSRC) # add user sources to include path
CPPSRC += $(call target_files,$(USRSRC_SLASH),*.cpp)
CSRC += $(call target_files,$(USRSRC_SLASH),*.c)

APPSOURCES=$(call target_files,$(USRSRC_SLASH),*.cpp)
APPSOURCES+=$(call target_files,$(USRSRC_SLASH),*.c)
 
# Custom stuff can be added here
# Include all directories in /lib/
INCLUDE_DIRS_LIB = $(wildcard $(APPDIR)/lib/*)
INCLUDE_DIRS += $(INCLUDE_DIRS_LIB)
# Add all src files in root of each library folder in /lib/
CPPSRC += $(patsubst $(APPDIR)/%,%,$(foreach %,$(INCLUDE_DIRS_LIB),$(wildcard $(%)/*.cpp)))
# Include all directories in /src/
INCLUDE_DIRS_SRC += $(sort $(dir $(wildcard $(APPDIR)/src/*/)))
INCLUDE_DIRS += $(INCLUDE_DIRS_SRC)

# Define constants to remove compilation warnings from these external libs:
#	-SparkFun_u-blox_GNSS_Arduino_Library
#	-Adafruit_MAX31855
#	-ArduinoJson
EXTRA_CFLAGS += '-DARDUINO=1'
#	-mcp2515_can
EXTRA_CFLAGS += '-DDEBUG_RXANY=0'

# Get rid of pragma region warnings
EXTRA_CFLAGS += '-Wno-unknown-pragmas'

