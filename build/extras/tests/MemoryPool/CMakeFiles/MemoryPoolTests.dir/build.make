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
include extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/compiler_depend.make

# Include the progress variables for this target.
include extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/progress.make

# Include the compile flags for this target's objects.
include extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/flags.make

extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/allocVariant.cpp.obj: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/flags.make
extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/allocVariant.cpp.obj: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/includes_CXX.rsp
extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/allocVariant.cpp.obj: C:/Supermileage/telemetry-firmware/lib/ArduinoJson/extras/tests/MemoryPool/allocVariant.cpp
extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/allocVariant.cpp.obj: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Supermileage\telemetry-firmware\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/allocVariant.cpp.obj"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/allocVariant.cpp.obj -MF CMakeFiles\MemoryPoolTests.dir\allocVariant.cpp.obj.d -o CMakeFiles\MemoryPoolTests.dir\allocVariant.cpp.obj -c C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\tests\MemoryPool\allocVariant.cpp

extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/allocVariant.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MemoryPoolTests.dir/allocVariant.cpp.i"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\tests\MemoryPool\allocVariant.cpp > CMakeFiles\MemoryPoolTests.dir\allocVariant.cpp.i

extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/allocVariant.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MemoryPoolTests.dir/allocVariant.cpp.s"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\tests\MemoryPool\allocVariant.cpp -o CMakeFiles\MemoryPoolTests.dir\allocVariant.cpp.s

extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/clear.cpp.obj: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/flags.make
extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/clear.cpp.obj: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/includes_CXX.rsp
extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/clear.cpp.obj: C:/Supermileage/telemetry-firmware/lib/ArduinoJson/extras/tests/MemoryPool/clear.cpp
extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/clear.cpp.obj: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Supermileage\telemetry-firmware\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/clear.cpp.obj"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/clear.cpp.obj -MF CMakeFiles\MemoryPoolTests.dir\clear.cpp.obj.d -o CMakeFiles\MemoryPoolTests.dir\clear.cpp.obj -c C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\tests\MemoryPool\clear.cpp

extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/clear.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MemoryPoolTests.dir/clear.cpp.i"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\tests\MemoryPool\clear.cpp > CMakeFiles\MemoryPoolTests.dir\clear.cpp.i

extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/clear.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MemoryPoolTests.dir/clear.cpp.s"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\tests\MemoryPool\clear.cpp -o CMakeFiles\MemoryPoolTests.dir\clear.cpp.s

extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/saveString.cpp.obj: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/flags.make
extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/saveString.cpp.obj: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/includes_CXX.rsp
extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/saveString.cpp.obj: C:/Supermileage/telemetry-firmware/lib/ArduinoJson/extras/tests/MemoryPool/saveString.cpp
extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/saveString.cpp.obj: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Supermileage\telemetry-firmware\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/saveString.cpp.obj"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/saveString.cpp.obj -MF CMakeFiles\MemoryPoolTests.dir\saveString.cpp.obj.d -o CMakeFiles\MemoryPoolTests.dir\saveString.cpp.obj -c C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\tests\MemoryPool\saveString.cpp

extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/saveString.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MemoryPoolTests.dir/saveString.cpp.i"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\tests\MemoryPool\saveString.cpp > CMakeFiles\MemoryPoolTests.dir\saveString.cpp.i

extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/saveString.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MemoryPoolTests.dir/saveString.cpp.s"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\tests\MemoryPool\saveString.cpp -o CMakeFiles\MemoryPoolTests.dir\saveString.cpp.s

extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/size.cpp.obj: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/flags.make
extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/size.cpp.obj: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/includes_CXX.rsp
extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/size.cpp.obj: C:/Supermileage/telemetry-firmware/lib/ArduinoJson/extras/tests/MemoryPool/size.cpp
extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/size.cpp.obj: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Supermileage\telemetry-firmware\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/size.cpp.obj"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/size.cpp.obj -MF CMakeFiles\MemoryPoolTests.dir\size.cpp.obj.d -o CMakeFiles\MemoryPoolTests.dir\size.cpp.obj -c C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\tests\MemoryPool\size.cpp

extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/size.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MemoryPoolTests.dir/size.cpp.i"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\tests\MemoryPool\size.cpp > CMakeFiles\MemoryPoolTests.dir\size.cpp.i

extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/size.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MemoryPoolTests.dir/size.cpp.s"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\tests\MemoryPool\size.cpp -o CMakeFiles\MemoryPoolTests.dir\size.cpp.s

extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/StringCopier.cpp.obj: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/flags.make
extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/StringCopier.cpp.obj: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/includes_CXX.rsp
extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/StringCopier.cpp.obj: C:/Supermileage/telemetry-firmware/lib/ArduinoJson/extras/tests/MemoryPool/StringCopier.cpp
extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/StringCopier.cpp.obj: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Supermileage\telemetry-firmware\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/StringCopier.cpp.obj"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/StringCopier.cpp.obj -MF CMakeFiles\MemoryPoolTests.dir\StringCopier.cpp.obj.d -o CMakeFiles\MemoryPoolTests.dir\StringCopier.cpp.obj -c C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\tests\MemoryPool\StringCopier.cpp

extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/StringCopier.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MemoryPoolTests.dir/StringCopier.cpp.i"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\tests\MemoryPool\StringCopier.cpp > CMakeFiles\MemoryPoolTests.dir\StringCopier.cpp.i

extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/StringCopier.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MemoryPoolTests.dir/StringCopier.cpp.s"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool && C:\msys64\ucrt64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\tests\MemoryPool\StringCopier.cpp -o CMakeFiles\MemoryPoolTests.dir\StringCopier.cpp.s

# Object files for target MemoryPoolTests
MemoryPoolTests_OBJECTS = \
"CMakeFiles/MemoryPoolTests.dir/allocVariant.cpp.obj" \
"CMakeFiles/MemoryPoolTests.dir/clear.cpp.obj" \
"CMakeFiles/MemoryPoolTests.dir/saveString.cpp.obj" \
"CMakeFiles/MemoryPoolTests.dir/size.cpp.obj" \
"CMakeFiles/MemoryPoolTests.dir/StringCopier.cpp.obj"

# External object files for target MemoryPoolTests
MemoryPoolTests_EXTERNAL_OBJECTS =

extras/tests/MemoryPool/MemoryPoolTests.exe: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/allocVariant.cpp.obj
extras/tests/MemoryPool/MemoryPoolTests.exe: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/clear.cpp.obj
extras/tests/MemoryPool/MemoryPoolTests.exe: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/saveString.cpp.obj
extras/tests/MemoryPool/MemoryPoolTests.exe: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/size.cpp.obj
extras/tests/MemoryPool/MemoryPoolTests.exe: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/StringCopier.cpp.obj
extras/tests/MemoryPool/MemoryPoolTests.exe: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/build.make
extras/tests/MemoryPool/MemoryPoolTests.exe: extras/tests/catch/libcatch.a
extras/tests/MemoryPool/MemoryPoolTests.exe: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/linkLibs.rsp
extras/tests/MemoryPool/MemoryPoolTests.exe: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/objects1.rsp
extras/tests/MemoryPool/MemoryPoolTests.exe: extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Supermileage\telemetry-firmware\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable MemoryPoolTests.exe"
	cd /d C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\MemoryPoolTests.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/build: extras/tests/MemoryPool/MemoryPoolTests.exe
.PHONY : extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/build

extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/clean:
	cd /d C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool && $(CMAKE_COMMAND) -P CMakeFiles\MemoryPoolTests.dir\cmake_clean.cmake
.PHONY : extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/clean

extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Supermileage\telemetry-firmware\lib\ArduinoJson C:\Supermileage\telemetry-firmware\lib\ArduinoJson\extras\tests\MemoryPool C:\Supermileage\telemetry-firmware\build C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool C:\Supermileage\telemetry-firmware\build\extras\tests\MemoryPool\CMakeFiles\MemoryPoolTests.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : extras/tests/MemoryPool/CMakeFiles/MemoryPoolTests.dir/depend

