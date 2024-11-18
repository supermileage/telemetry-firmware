# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Supermileage\telemetry-firmware\lib\ArduinoJson

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Supermileage\telemetry-firmware\build

# Include any dependencies generated for this target.
include extras/fuzzing/CMakeFiles/json_reproducer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include extras/fuzzing/CMakeFiles/json_reproducer.dir/compiler_depend.make

# Include the progress variables for this target.
include extras/fuzzing/CMakeFiles/json_reproducer.dir/progress.make

# Include the compile flags for this target's objects.
include extras/fuzzing/CMakeFiles/json_reproducer.dir/flags.make

extras/fuzzing/CMakeFiles/json_reproducer.dir/json_fuzzer.cpp.obj: extras/fuzzing/CMakeFiles/json_reproducer.dir/flags.make
extras/fuzzing/CMakeFiles/json_reproducer.dir/json_fuzzer.cpp.obj: extras/fuzzing/CMakeFiles/json_reproducer.dir/includes_CXX.rsp
extras/fuzzing/CMakeFiles/json_reproducer.dir/json_fuzzer.cpp.obj: C:/Supermileage/telemetry-firmware/lib/ArduinoJson/extras/fuzzing/json_fuzzer.cpp
extras/fuzzing/CMakeFiles/json_reproducer.dir/json_fuzzer.cpp.obj: extras/fuzzing/CMakeFiles/json_reproducer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Supermileage\telemetry-firmware\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object extras/fuzzing/CMakeFiles/json_reproducer.dir/json_fuzzer.cpp.obj"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\fuzzing && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT extras/fuzzing/CMakeFiles/json_reproducer.dir/json_fuzzer.cpp.obj -MF CMakeFiles\json_reproducer.dir\json_fuzzer.cpp.obj.d -o CMakeFiles\json_reproducer.dir\json_fuzzer.cpp.obj -c C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\fuzzing\json_fuzzer.cpp

extras/fuzzing/CMakeFiles/json_reproducer.dir/json_fuzzer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/json_reproducer.dir/json_fuzzer.cpp.i"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\fuzzing && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\fuzzing\json_fuzzer.cpp > CMakeFiles\json_reproducer.dir\json_fuzzer.cpp.i

extras/fuzzing/CMakeFiles/json_reproducer.dir/json_fuzzer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/json_reproducer.dir/json_fuzzer.cpp.s"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\fuzzing && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\fuzzing\json_fuzzer.cpp -o CMakeFiles\json_reproducer.dir\json_fuzzer.cpp.s

extras/fuzzing/CMakeFiles/json_reproducer.dir/reproducer.cpp.obj: extras/fuzzing/CMakeFiles/json_reproducer.dir/flags.make
extras/fuzzing/CMakeFiles/json_reproducer.dir/reproducer.cpp.obj: extras/fuzzing/CMakeFiles/json_reproducer.dir/includes_CXX.rsp
extras/fuzzing/CMakeFiles/json_reproducer.dir/reproducer.cpp.obj: C:/Supermileage/telemetry-firmware/lib/ArduinoJson/extras/fuzzing/reproducer.cpp
extras/fuzzing/CMakeFiles/json_reproducer.dir/reproducer.cpp.obj: extras/fuzzing/CMakeFiles/json_reproducer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Supermileage\telemetry-firmware\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object extras/fuzzing/CMakeFiles/json_reproducer.dir/reproducer.cpp.obj"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\fuzzing && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT extras/fuzzing/CMakeFiles/json_reproducer.dir/reproducer.cpp.obj -MF CMakeFiles\json_reproducer.dir\reproducer.cpp.obj.d -o CMakeFiles\json_reproducer.dir\reproducer.cpp.obj -c C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\fuzzing\reproducer.cpp

extras/fuzzing/CMakeFiles/json_reproducer.dir/reproducer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/json_reproducer.dir/reproducer.cpp.i"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\fuzzing && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\fuzzing\reproducer.cpp > CMakeFiles\json_reproducer.dir\reproducer.cpp.i

extras/fuzzing/CMakeFiles/json_reproducer.dir/reproducer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/json_reproducer.dir/reproducer.cpp.s"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\fuzzing && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\fuzzing\reproducer.cpp -o CMakeFiles\json_reproducer.dir\reproducer.cpp.s

# Object files for target json_reproducer
json_reproducer_OBJECTS = \
"CMakeFiles/json_reproducer.dir/json_fuzzer.cpp.obj" \
"CMakeFiles/json_reproducer.dir/reproducer.cpp.obj"

# External object files for target json_reproducer
json_reproducer_EXTERNAL_OBJECTS =

extras/fuzzing/json_reproducer.exe: extras/fuzzing/CMakeFiles/json_reproducer.dir/json_fuzzer.cpp.obj
extras/fuzzing/json_reproducer.exe: extras/fuzzing/CMakeFiles/json_reproducer.dir/reproducer.cpp.obj
extras/fuzzing/json_reproducer.exe: extras/fuzzing/CMakeFiles/json_reproducer.dir/build.make
extras/fuzzing/json_reproducer.exe: extras/fuzzing/CMakeFiles/json_reproducer.dir/linkLibs.rsp
extras/fuzzing/json_reproducer.exe: extras/fuzzing/CMakeFiles/json_reproducer.dir/objects1.rsp
extras/fuzzing/json_reproducer.exe: extras/fuzzing/CMakeFiles/json_reproducer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Supermileage\telemetry-firmware\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable json_reproducer.exe"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\fuzzing && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\json_reproducer.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
extras/fuzzing/CMakeFiles/json_reproducer.dir/build: extras/fuzzing/json_reproducer.exe
.PHONY : extras/fuzzing/CMakeFiles/json_reproducer.dir/build

extras/fuzzing/CMakeFiles/json_reproducer.dir/clean:
	cd /d C:\Supermileage\telemetry-firmware\build\extras\fuzzing && $(CMAKE_COMMAND) -P CMakeFiles\json_reproducer.dir\cmake_clean.cmake
.PHONY : extras/fuzzing/CMakeFiles/json_reproducer.dir/clean

extras/fuzzing/CMakeFiles/json_reproducer.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Supermileage\telemetry-firmware\lib\ArduinoJson C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\fuzzing C:\Supermileage\telemetry-firmware\build C:\Supermileage\telemetry-firmware\build\extras\fuzzing C:\Supermileage\telemetry-firmware\build\extras\fuzzing\CMakeFiles\json_reproducer.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : extras/fuzzing/CMakeFiles/json_reproducer.dir/depend

