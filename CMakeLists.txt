cmake_minimum_required(VERSION 3.16)
add_subdirectory($ENV{MCU_SERVER_PATH} ${CMAKE_BINARY_DIR}/mcu_server)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp-server)