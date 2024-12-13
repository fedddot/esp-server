cmake_minimum_required(VERSION 3.16)

set(TARGET "esp32s2")
set(CMAKE_TOOLCHAIN_FILE $ENV{IDF_PATH}/tools/cmake/toolchain-${TARGET}.cmake)
project(esp-server C CXX)

include($ENV{IDF_PATH}/tools/cmake/idf.cmake)

idf_build_process(
    ${TARGET}
    COMPONENTS freertos esptool_py
    SDKCONFIG ${CMAKE_CURRENT_LIST_DIR}/sdkconfig
    BUILD_DIR ${CMAKE_BINARY_DIR}
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(esp-server.elf ${CMAKE_CURRENT_LIST_DIR}/src/esp-server.cpp)
target_link_libraries(esp-server.elf idf::freertos idf::spi_flash cnc_server test_ipc_connection)
idf_build_executable(esp-server.elf)

add_subdirectory($ENV{MCU_SERVER_PATH} ${CMAKE_BINARY_DIR}/mcu_server)