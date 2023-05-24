# Install script for directory: C:/Users/Martin/Documents/git/pico-examples

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/pico_examples")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2021.10/bin/arm-none-eabi-objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/Martin/Documents/git/pico-examples/pico-sdk/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/blink/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/hello_world/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/adc/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/clocks/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/cmake/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/divider/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/dma/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/flash/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/gpio/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/i2c/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/interp/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/multicore/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/picoboard/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/pico_w/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/pio/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/pwm/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/reset/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/rtc/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/spi/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/system/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/timer/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/uart/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/usb/cmake_install.cmake")
  include("C:/Users/Martin/Documents/git/pico-examples/watchdog/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/Martin/Documents/git/pico-examples/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
