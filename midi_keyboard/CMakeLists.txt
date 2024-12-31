# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(midi_keyboard C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

set(DEBUG_USB 0)

if (DEBUG_USB)
add_compile_definitions(DEBUG_USB)
endif()

# Add executable. Default name is the project name, version 0.1

set(SOURCES midi_keyboard.cpp KeyboardKey.cpp)
if (NOT DEBUG_USB)
    list(APPEND SOURCES usb_descriptors.c)
endif()
add_executable(midi_keyboard ${SOURCES})

pico_set_program_name(midi_keyboard "midi_keyboard")
pico_set_program_version(midi_keyboard "0.1")

# Generate PIO header
pico_generate_pio_header(midi_keyboard ${CMAKE_CURRENT_LIST_DIR}/buttonmatrix.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(midi_keyboard 0)

if (DEBUG_USB)
pico_enable_stdio_usb(midi_keyboard 1)
else()
pico_enable_stdio_usb(midi_keyboard 0)
endif()

# Add the standard library to the build
target_link_libraries(midi_keyboard
        pico_stdlib
        hardware_irq
        hardware_pio)

target_include_directories(midi_keyboard PRIVATE ${CMAKE_CURRENT_LIST_DIR})

if (NOT DEBUG_USB)
    target_include_directories(midi_keyboard PRIVATE ${CMAKE_CURRENT_LIST_DIR}/usbmidi)

    target_link_libraries(midi_keyboard 
            tinyusb_device
            tinyusb_board
        )
endif()

pico_add_extra_outputs(midi_keyboard)

