# Install script for directory: C:/Supermileage/telemetry-firmware/lib/ArduinoJson/extras/tests

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/ArduinoJson")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/msys64/ucrt64/bin/objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/catch/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/Cpp11/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/Cpp17/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/Cpp20/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/FailingBuilds/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/IntegrationTests/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/JsonArray/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/JsonDeserializer/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/JsonDocument/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/JsonObject/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/JsonSerializer/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/JsonVariant/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/MemoryPool/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/Misc/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/MixedConfiguration/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/MsgPackDeserializer/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/MsgPackSerializer/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/Numbers/cmake_install.cmake")
  include("C:/Supermileage/telemetry-firmware/build/extras/tests/TextFormatter/cmake_install.cmake")

endif()

