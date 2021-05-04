# Standard behavior must be included here
INCLUDE_DIRS += $(SOURCE_PATH)/$(USRSRC) # add user sources to include path
CPPSRC += $(call target_files,$(USRSRC_SLASH),*.cpp)
CSRC += $(call target_files,$(USRSRC_SLASH),*.c)
 
APPSOURCES=$(call target_files,$(USRSRC_SLASH),*.cpp)
APPSOURCES+=$(call target_files,$(USRSRC_SLASH),*.c)
 
# Custom stuff can be added here
INCLUDE_DIRS += $(SOURCE_PATH)lib/Adafruit-MAX31855/
INCLUDE_DIRS += $(SOURCE_PATH)lib/Adafruit-BusIO/
INCLUDE_DIRS += $(SOURCE_PATH)lib/MCP_CAN_lib/
CPPSRC += $(call target_files,lib/Adafruit-MAX31855/,*.cpp)
CPPSRC += $(call target_files,lib/Adafruit-BusIO/,*.cpp)
CPPSRC += $(call target_files,lib/MCP_CAN_lib/,*.cpp)