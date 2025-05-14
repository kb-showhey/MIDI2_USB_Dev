cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME MIDI2_USB_Dev)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Libs/tinyusb/src フォルダ内のすべての .c ファイルを取得
file(GLOB_RECURSE DRIVER_SRC_FILES "${CMAKE_SOURCE_DIR}/Libs/tinyusb/src/*.c")

# 除外するファイルを指定
list(REMOVE_ITEM DRIVER_SRC_FILES "${CMAKE_SOURCE_DIR}/Libs/tinyusb/src/device/usbd.c")

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${CMAKE_SOURCE_DIR}/Core/Src/midi_proc.cpp
    ${CMAKE_SOURCE_DIR}/Core/Src/usb_descriptors.c
    ${DRIVER_SRC_FILES}
    ${CMAKE_SOURCE_DIR}/Libs/tusb_ump/ump_device.cpp
    ${CMAKE_SOURCE_DIR}/Libs/usbd.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ${CMAKE_SOURCE_DIR}/Core/Inc
    ${CMAKE_SOURCE_DIR}/Libs/tinyusb/hw
    ${CMAKE_SOURCE_DIR}/Libs/tinyusb/src
    ${CMAKE_SOURCE_DIR}/Libs/tusb_ump
    ${CMAKE_SOURCE_DIR}/Libs/AM_MIDI2.0Lib/include
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
