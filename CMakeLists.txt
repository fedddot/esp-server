cmake_minimum_required(VERSION 3.20)

set(TARGET "esp32s2")
set(CMAKE_TOOLCHAIN_FILE "$ENV{IDF_PATH}/tools/cmake/toolchain-${TARGET}.cmake")

project(esp_mcu_server C CXX ASM)

# Include for ESP-IDF build system functions
include($ENV{IDF_PATH}/tools/cmake/idf.cmake)

# Create idf::{target} and idf::freertos static libraries
idf_build_process(
    "esp32s2"
    COMPONENTS freertos esptool_py
    SDKCONFIG ${CMAKE_CURRENT_LIST_DIR}/sdkconfig
    BUILD_DIR ${CMAKE_BINARY_DIR}
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(elf_file ${CMAKE_PROJECT_NAME}.elf)
add_executable(${elf_file} ${CMAKE_CURRENT_LIST_DIR}/src/esp-server.cpp)

target_link_libraries(${elf_file} idf::freertos idf::spi_flash)
idf_build_executable(${elf_file})